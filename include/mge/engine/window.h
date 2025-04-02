/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include <sdl3/sdl.h>

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>

typedef struct SDL_KeyboardUpdate
{
	const bool* down;

} SDL_KeyboardUpdate;

typedef struct SDL_MouseUpdate
{
	int32_t x, y;
	int32_t xrel, yrel;

} SDL_MouseUpdate;

typedef struct SDL_GamepadUpdate
{
	// @todo

} SDL_GamepadUpdate;

typedef enum SDL_UpdateType
{
	SDL_UPDATE_KEYBOARD,
	SDL_UPDATE_MOUSE,
	SDL_UPDATE_GAMEPAD,

} SDL_UpdateType;

typedef union SDL_Update
{
	SDL_KeyboardUpdate keyboard;
	SDL_MouseUpdate mouse;
	SDL_GamepadUpdate gamepad;

} SDL_Update;

namespace vr
{
	///
	typedef std::function<void(const SDL_Event&)> EventCallback;

	///
	typedef std::function<void(const SDL_Update&)> UpdateCallback;

	///
	class Window : public std::enable_shared_from_this<Window>
	{
	public:
		Window(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags);
		~Window();

		///
		friend std::shared_ptr<Window> createWindow(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags);
		
		/// 
		bool isClosed();

		/// 
		void registerEvent(uint32_t eventType, EventCallback callback);

		/// 
		void registerUpdate(uint32_t updateType, UpdateCallback callback);

		///		
		void setCursorVisible(bool _visible);

		///
		void setCursorLock(bool _lock);
		
		///
		void setCursorPos(int32_t _x, int32_t _y);

		///		
		void setFullscreen(bool _fullscreen);

		///		
		uint32_t getWidth();

		///		
		uint32_t getHeight();

		///		
		void* getNativeHandle();

		/// 
		void* getNativeDisplayHandle();

	private:
		bool quit;
		SDL_Window* window;

		std::unordered_map<uint32_t, std::vector<EventCallback>> pushEvents;
		std::unordered_map<uint32_t, std::vector<UpdateCallback>> updateEvents;
	};

} // namespace vr