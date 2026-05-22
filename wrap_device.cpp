#include "wrap.h"
#include "wrap_internal.h"

#include <algorithm>
#include <cassert>
#include <expected>
#include <vector>
#include <stdfloat>
#include <vulkan/vulkan_core.h>

std::expected<DrawingDeviceInfo, char const*> pickDrawingDevice(
	VkInstance instance, VkSurfaceKHR surface, 
	std::span<char const*> requiredExtensions) {

	// Enumerate physical devices
	uint32_t physicalDeviceCount = 0;
	assert(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr)
		== VK_SUCCESS);
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	assert(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, 
		physicalDevices.data()) == VK_SUCCESS);

	for (auto const physicalDevice : physicalDevices) {
		DrawingDeviceInfo deviceInfo{};
		deviceInfo.physical = physicalDevice;

		// Get the device name
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		char const* tmpName = deviceProperties.deviceName;
		deviceInfo.name = tmpName; // copy assign to owned value

		// Ensure device supports required extensions
		uint32_t extensionCount = 0;
		assert(vkEnumerateDeviceExtensionProperties(physicalDevice, 
			nullptr, &extensionCount, nullptr) == VK_SUCCESS);
		std::vector<VkExtensionProperties> availableExtensionBufStructs(
				extensionCount);
		assert(vkEnumerateDeviceExtensionProperties(physicalDevice, 
			nullptr, &extensionCount, availableExtensionBufStructs.data()) 
			== VK_SUCCESS);
		std::vector<char const*> availableExtensions(extensionCount);
		std::ranges::transform(availableExtensionBufStructs, 
			availableExtensions.begin(), 
			[](VkExtensionProperties const& props) {
			return props.extensionName;
		});
		bool haveAllExtensions = areRequirementsSatisfied(requiredExtensions,
				availableExtensions);
		if (!haveAllExtensions) {
			continue;
		}

		// Test if a device has required queues
		uint32_t familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, 
			&familyCount, nullptr);
		std::vector<VkQueueFamilyProperties> families(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, 
			&familyCount, families.data());
		std::optional<uint32_t> graphicsIdx, presentIdx;
		for (size_t i = 0; i < familyCount; i ++) {
			auto const family = families[i];
			// graphics
			if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) > 0)
				graphicsIdx = i;
			// presentation
			VkBool32 supportsPresentation = false;
			assert(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, 
				surface, &supportsPresentation) == VK_SUCCESS);
			if (supportsPresentation == VK_TRUE) {
				presentIdx = i;
			}
		}
		if (!graphicsIdx.has_value()) continue;
		if (!presentIdx.has_value()) continue;
		deviceInfo.graphicsQueueIndex = graphicsIdx.value();
		deviceInfo.presentQueueIndex = presentIdx.value();
		deviceInfo.allQueueIndices.push_back(deviceInfo.graphicsQueueIndex);
		if (deviceInfo.presentQueueIndex != deviceInfo.graphicsQueueIndex)
			deviceInfo.allQueueIndices.push_back(deviceInfo.presentQueueIndex);

		float const priority = 1.0f;
		std::vector<VkDeviceQueueCreateInfo> queueCIs(
			deviceInfo.allQueueIndices.size());
		for (size_t i = 0; i < deviceInfo.allQueueIndices.size(); i ++) {
			uint32_t familyIndex = deviceInfo.allQueueIndices[i];
			queueCIs[i] = VkDeviceQueueCreateInfo{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = familyIndex,
				.queueCount = 1,
				.pQueuePriorities = &priority,
			};
		}

		// Logical device creation
		VkPhysicalDeviceFeatures requiredFeatures{};
		VkDeviceCreateInfo deviceCI {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = static_cast<uint32_t>(queueCIs.size()),
			.pQueueCreateInfos = queueCIs.data(),
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = static_cast<uint32_t>(
					requiredExtensions.size()),
			.ppEnabledExtensionNames = requiredExtensions.data(),
			.pEnabledFeatures = &requiredFeatures,
		};
		assert(vkCreateDevice(physicalDevice, &deviceCI, nullptr, 
			&deviceInfo.logical) == VK_SUCCESS);
		return deviceInfo;
	}

	return std::unexpected("Could not find a valid render device!");
}
