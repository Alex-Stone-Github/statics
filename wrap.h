#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <expected>
#include <string>
#include <array>
#include <span>
#include <cstdint>
#include <vulkan/vulkan_core.h>

inline std::array<char const*, 1> instanceDebugLayers = {
	"VK_LAYER_KHRONOS_validation",
};
inline std::array<char const*, 1> deviceSwapchainExtensions = {
	"VK_KHR_swapchain",
};
inline std::array<char const*, 0> noExtensions = {};

/// This is a drawing device, you would use one of these to draw to a drawing 
/// surface. You can create multiple drawing devices that point to the same 
/// physical device or have a compute device that also points to an identical
/// drawing device.
struct DrawingDeviceInfo {
	VkPhysicalDevice physical;
	VkDevice logical;
	std::string name;

	std::vector<uint32_t> allQueueIndices;
	uint32_t graphicsQueueIndex;
	VkQueue graphicsQueue;
	uint32_t presentQueueIndex;
	VkQueue presentQueue;
};

// May want to render to multiple spots
struct DrawingSurfaceInfo {
	VkSwapchainKHR swapchain;

	// Per Swapchain Images
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	std::vector<VkFramebuffer> swapchainImageFramebuffers;
	std::vector<VkSemaphore> frameFinishedSemaphores;

	// Per Frame-In-Flight
	std::vector<VkFence> frameInFlightFences;
	std::vector<VkSemaphore> frameAvailableSemaphores;
};

/// Get a vulkan instance
std::expected<VkInstance, char const*> getInstance(
	std::span<char const*> requiredValidationLayers,
	std::span<char const*> requiredExtensions,
	char const* applicationName);

/// Get a vulkan device
std::expected<DrawingDeviceInfo, char const*> pickDrawingDevice(
	VkInstance instance, VkSurfaceKHR surface, 
	std::span<char const*> requiredExtensions);

/// Renering target
std::expected<DrawingSurfaceInfo, char const*> constructDrawingSurface(
	DrawingDeviceInfo device, VkSurfaceKHR surface, VkExtent2D dimensions, 
	VkSwapchainKHR movedSwapchain);

