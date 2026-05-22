#include <optional>
#include <print>
#include <array>
#include <cstdlib>
#include <cstdlib>
#include <span>
#include <vulkan/vulkan_core.h>

#include "wrap.h"
#include "wrap_sdlctx.h"

/// What does a writer look like?
template <typename T>
concept Writable = requires(T example, std::span<std::byte> bytes) {
	{ example.write(bytes) } -> std::same_as<size_t>;
};


/// Let's create an example writer object
struct Stdout {
	size_t write(std::span<std::byte>) {
		return 0;
	}
};


/// Lets take in a writer and do some stuff with it
void writesomestuff(Writable auto writer) {
	// Get some bytes and write some stuff
	std::array<std::byte, 3> arr = {std::byte{3}, std::byte{4}, std::byte{5}};
	writer.write(arr);
}

int main() {
	// Print
	std::println("Hello, World!");

	// Get a window
	SDLWindowInfo window = SDLWindowFactory::getInstance()
		.createWindow(800, 600, "somewin");
	std::println("I have a genuine sdl window!");

	// Get a instance
	VkInstance instance = getInstance(instanceDebugLayers, 
		window.requiredExtensions, "Statics Testing Application").value();
	std::println("I have a vulkan instance!");

	// Get a surface
	VkSurfaceKHR surface = window.createSurface(instance);
	std::println("I have a window surface!");

	// Get a device
	DrawingDeviceInfo device = pickDrawingDevice(instance, surface, 
		deviceSwapchainExtensions).value();
	std::println("I have a vulkan render device, name = '{}'", device.name);

	// Drawing surface
	DrawingSurfaceInfo surfaceInfo = constructDrawingSurface(device, 
			surface, window.getSize(), nullptr).value();
	std::println("I have a vulkan render surface!");

	return EXIT_SUCCESS;
}
