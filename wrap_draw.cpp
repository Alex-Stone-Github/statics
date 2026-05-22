#include "wrap.h"

#include <cstdint>
#include <expected>
#include <vector>
#include <vulkan/vulkan.h>
#include <cassert>
#include <optional>
#include <print>

std::expected<DrawingSurfaceInfo, char const*> constructDrawingSurface(
	DrawingDeviceInfo device, VkSurfaceKHR surface, VkExtent2D dimensions, 
	VkSwapchainKHR movedSwapchain) {
	DrawingSurfaceInfo surfaceInfo{};

	// How many swapchain images?
	VkSurfaceCapabilitiesKHR surfaceCapabilities{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.physical, surface,
		&surfaceCapabilities);
	uint32_t maxImages = surfaceCapabilities.maxImageCount;
	if (maxImages == 0) maxImages = UINT32_MAX;
	uint32_t imageCount = std::min(
			maxImages, surfaceCapabilities.minImageCount + 1);

	// What format // colorspace?
	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device.physical, surface, 
		&surfaceFormatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device.physical, surface, 
		&surfaceFormatCount, surfaceFormats.data());
	std::optional<VkSurfaceFormatKHR> selectedSurfaceFormat;
	for (auto const& surfaceFormat : surfaceFormats) {
		if (surfaceFormat.format != VK_FORMAT_B8G8R8A8_SRGB) continue;
		if (surfaceFormat.colorSpace != VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
			continue;
		selectedSurfaceFormat = surfaceFormat;
	}
	if (!selectedSurfaceFormat.has_value()) 
		return std::unexpected("Could not find acceptable surfaceFormat!");

	bool uniqueQueue = device.allQueueIndices.size() == 1;

	VkSwapchainCreateInfoKHR swapchainCI {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = imageCount,
		.imageFormat = selectedSurfaceFormat->format,
		.imageColorSpace = selectedSurfaceFormat->colorSpace,
		.imageExtent = dimensions,
		.imageArrayLayers = 1, // not doing vr
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = uniqueQueue? VK_SHARING_MODE_EXCLUSIVE : 
			VK_SHARING_MODE_CONCURRENT,
		.queueFamilyIndexCount = uniqueQueue? 0 : 
			static_cast<uint32_t>(device.allQueueIndices.size()),
		.pQueueFamilyIndices = uniqueQueue? nullptr :
			device.allQueueIndices.data(),
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // no compositor
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.clipped = VK_TRUE,
		.oldSwapchain = movedSwapchain,
	};
	vkCreateSwapchainKHR(device.logical, &swapchainCI, nullptr, 
		&surfaceInfo.swapchain);
	return surfaceInfo;
}
