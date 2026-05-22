#include "wrap_sdlctx.h"
#include "SDL.h"
#include "SDL_video.h"

#include <SDL2/SDL_vulkan.h>
#include <cassert>
#include <print>


SDLWindowInfo::~SDLWindowInfo() {
	SDL_DestroyWindow(window);
}
VkSurfaceKHR SDLWindowInfo::createSurface(VkInstance instance) const {
	VkSurfaceKHR surface;
	SDL_Vulkan_CreateSurface(window, instance, &surface);
	return surface;
}
VkExtent2D SDLWindowInfo::getSize() const {
	int32_t w, h;
	SDL_GetWindowSize(window, &w, &h);
	return VkExtent2D{static_cast<uint32_t>(w), static_cast<uint32_t>(h)};
}

SDLWindowFactory::SDLWindowFactory() {
	std::println("I am currently statically initing SDL2!!!!!");
	assert(SDL_Init(SDL_INIT_VIDEO) == 0);
	std::println("My sdl video driver is {}", SDL_GetCurrentVideoDriver());

}
SDLWindowInfo SDLWindowFactory::createWindow(uint32_t width, uint32_t height, 
			char const* title) {
	SDLWindowInfo windowInfo{};
	windowInfo.window = SDL_CreateWindow(title, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		width, height, SDL_WINDOW_VULKAN);
	uint32_t extensionCount = 0;
	SDL_Vulkan_GetInstanceExtensions(windowInfo.window, &extensionCount, 
		nullptr);
	windowInfo.requiredExtensions = std::vector<char const*>(extensionCount);
	SDL_Vulkan_GetInstanceExtensions(windowInfo.window, &extensionCount, 
		windowInfo.requiredExtensions.data());
	return windowInfo;
}
SDLWindowFactory::~SDLWindowFactory() {
	SDL_Quit();
}
