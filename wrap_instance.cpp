#include <algorithm>
#include <vector>
#include <cassert>
#include <vulkan/vulkan_core.h>

#include "wrap.h"
#include "wrap_internal.h"

std::expected<VkInstance, char const*> getInstance(
	std::span<char const*> requiredValidationLayers,
	std::span<char const*> requiredExtensions,
	char const* applicationName) {
	// Get the available properties
	uint32_t availableLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
	std::vector<VkLayerProperties> availableLayerBufStructs(
		availableLayerCount);
	vkEnumerateInstanceLayerProperties(&availableLayerCount, 
		availableLayerBufStructs.data());
	std::vector<char const*> availableValidationLayers(
			availableLayerBufStructs.size());
	std::ranges::transform(availableLayerBufStructs, 
		availableValidationLayers.begin(),
		[](VkLayerProperties const& props){
		return props.layerName;
	});
	bool haveAllValidationLayers = areRequirementsSatisfied(
			requiredValidationLayers, 
			availableValidationLayers);
	if (!haveAllValidationLayers) {
		return std::unexpected("Could not find all validation layers!");
	}

	// Get the available extensions
	uint32_t availableExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, 
		nullptr);
	std::vector<VkExtensionProperties> availableExtensionBufStructs(
		availableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, 
		availableExtensionBufStructs.data());
	std::vector<char const*> availableExtensions(availableExtensionCount);
	std::ranges::transform(availableExtensionBufStructs, 
		availableExtensions.begin(), [](VkExtensionProperties const& props) {
		return props.extensionName;
	});
	bool haveAllExtensions = areRequirementsSatisfied(requiredExtensions, 
		availableExtensions);
	if (!haveAllExtensions) {
		return std::unexpected("Could not find all required extensions!");
	}

	VkApplicationInfo appInfo {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = applicationName,
		.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
		.pEngineName = "Wrap Engine",
		.engineVersion = VK_MAKE_VERSION(0, 0, 0),
		.apiVersion = VK_API_VERSION_1_0,
	};
	VkInstanceCreateInfo instanceCI {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(
			requiredValidationLayers.size()),
		.ppEnabledLayerNames = requiredValidationLayers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(
				requiredExtensions.size()),
		.ppEnabledExtensionNames = requiredExtensions.data(),
	};
	VkInstance instance;
	assert(vkCreateInstance(&instanceCI, nullptr, &instance) == VK_SUCCESS);
	return instance;
}
