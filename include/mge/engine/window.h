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
	// @todo Implement...

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

namespace mge
{
	/// Event callback, primarily used push data like window resize, key pressed etc.
	///
	/// param[in] SDL_Event Event data.
	/// 
	typedef std::function<void(const SDL_Event&)> EventCallback;

	/// Update callback, primarily used for updated data like mouse move, gamepad axis etc.
	///
	/// param[in] SDL_Update Update data.
	/// 
	typedef std::function<void(const SDL_Update&)> UpdateCallback;

	/// Window and Event systems.
	///
	class Window : public std::enable_shared_from_this<Window>
	{
	public:
		Window(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags);
		~Window();

		/// Create a window and event context.
		/// 
		/// @param[in] _title Title to display on window.
		/// @param[in] _width Width of the window.
		/// @param[in] _height Height of the window.
		/// @param[in] _flags Window creation flags (see `SDL_WINDOW_*`.)
		/// 
		/// @returns Shared Window.
		/// 
		friend std::shared_ptr<Window> createWindow(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags);
		
		/// Checks if window should be closed.
		///
		/// @remark Should be called every frame in loop.
		/// 
		/// @returns True if window is closed.
		/// 
		bool isClosed();

		/// Register an event callback.
		/// 
		/// @param[in] eventType Type of event to register.
		/// @param[in] callback Function to be called when the event occurs.
		/// 
		void registerEvent(uint32_t _type, EventCallback _callback);

		/// Register an update callback.
		/// 
		/// @param[in] updateType Type of update to register.
		/// @param[in] callback Function to be called during updates.
		/// 
		void registerUpdate(uint32_t _type, UpdateCallback _callback);

		/// Show or hide the cursor.
		/// 
		/// @param[in] _visible Set to `true` to show the cursor, `false` to hide it.
		/// 
		void setCursorVisible(bool _visible);

		/// Lock or unlock the cursor to the window.
		/// 
		/// @param[in] _lock Set to `true` to lock the cursor, `false` to unlock it.
		/// 
		void setCursorLock(bool _lock);

		/// Set the cursor position within the window.
		/// 
		/// @param[in] _x X-coordinate relative to the window.
		/// @param[in] _y Y-coordinate relative to the window.
		/// 
		void setCursorPos(int32_t _x, int32_t _y);

		/// Enable or disable fullscreen mode.
		/// 
		/// @param[in] _fullscreen Set to `true` for fullscreen, `false` for windowed mode.
		/// 
		void setFullscreen(bool _fullscreen);

		/// Get the window width.
		/// 
		/// @returns The width of the window in pixels.
		/// 
		uint32_t getWidth();

		/// Get the window height.
		/// 
		/// @returns The height of the window in pixels.
		/// 
		uint32_t getHeight();

		/// Get the native window handle.
		/// 
		/// @returns A pointer to the native window handle.
		/// 
		void* getNativeHandle();

		/// Get the native display handle.
		/// 
		/// @returns A pointer to the native display handle.
		/// 
		void* getNativeDisplayHandle();

	private:
		bool quit;
		SDL_Window* window;

		std::unordered_map<uint32_t, std::vector<EventCallback>> pushEvents;
		std::unordered_map<uint32_t, std::vector<UpdateCallback>> updateEvents;
	};

} // namespace mge