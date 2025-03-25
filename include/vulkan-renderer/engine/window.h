/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <unordered_map>

namespace vr
{
	class Window
	{
	public:
		Window(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags);
		~Window();

		///
		friend std::shared_ptr<Window> createWindow(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags);

		///		
		void* getNativeHandle();

		/// 
		bool isClosed();

		///		
		bool isKeyDown(SDL_Scancode _code);

		///		
		void Window::getRelativeMouseState(float* _x, float* _y);

		///		
		void setCursorVisible(bool _visible);

		///		
		void setFullscreen(bool _fullscreen);

		///		
		SDL_WindowFlags getFlags();

		///		
		uint32_t getWidth();

		///		
		uint32_t getHeight();

	private:
		bool running;

		SDL_Window* window;
	};

} // namespace vr