#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include <vector>

struct SDLWindowInfo {
	SDL_Window* window;
	std::vector<char const*> requiredExtensions;
	SDLWindowInfo() = default;
	VkSurfaceKHR createSurface(VkInstance instance) const;
	VkExtent2D getSize() const;
	SDLWindowInfo(SDLWindowInfo const&) = delete;
	SDLWindowInfo& operator=(SDLWindowInfo const&) = delete;
	SDLWindowInfo(SDLWindowInfo&& other) {
		std::swap(requiredExtensions, other.requiredExtensions);
		std::swap(window, other.window);
	}
	SDLWindowInfo& operator=(SDLWindowInfo&& other) {
		std::swap(requiredExtensions, other.requiredExtensions);
		std::swap(window, other.window);
		return *this;
	}
	~SDLWindowInfo();
};

class SDLWindowFactory {
public:
	static SDLWindowFactory& getInstance() {
		static SDLWindowFactory instance{};
		return instance;
	}
	SDLWindowInfo createWindow(uint32_t width, uint32_t height, 
			char const* title);
private:
	SDLWindowFactory();
	SDLWindowFactory(SDLWindowFactory const&) = delete;
	SDLWindowFactory& operator=(SDLWindowFactory const&) = delete;
	SDLWindowFactory(SDLWindowFactory&&) = delete;
	SDLWindowFactory& operator=(SDLWindowFactory&&) = delete;
	~SDLWindowFactory();
};
