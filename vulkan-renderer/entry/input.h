#pragma once

#include "entry.h"

namespace vr 
{
	typedef void (*InputBindingFn)(const void* _userData);

	struct InputBinding
	{
		void set(vr::Key::Enum _key, uint8_t _modifiers, uint8_t _flags, InputBindingFn _fn, const void* _userData = NULL)
		{
			m_key = _key;
			m_modifiers = _modifiers;
			m_flags     = _flags;
			m_fn        = _fn;
			m_userData  = _userData;
		}

		void end()
		{
			m_key = vr::Key::None;
			m_modifiers = vr::Modifier::None;
			m_flags     = 0;
			m_fn        = NULL;
			m_userData  = NULL;
		}

		vr::Key::Enum m_key;
		uint8_t m_modifiers;
		uint8_t m_flags;
		InputBindingFn m_fn;
		const void* m_userData;
	};

	#define INPUT_BINDING_END { vr::Key::None, vr::Modifier::None, 0, NULL, NULL }

	///
	void inputInit();

	///
	void inputShutdown();

	///
	void inputAddBindings(const char* _name, const InputBinding* _bindings);

	///
	void inputRemoveBindings(const char* _name);

	///
	void inputProcess();

	///
	void inputSetKeyState(vr::Key::Enum  _key, uint8_t _modifiers, bool _down);

	///
	bool inputGetKeyState(vr::Key::Enum _key, uint8_t* _modifiers = NULL);

	///
	uint8_t inputGetModifiersState();

	/// Adds single UTF-8 encoded character into input buffer.
	void inputChar(uint8_t _len, const uint8_t _char[4]);

	/// Returns single UTF-8 encoded character from input buffer.
	const uint8_t* inputGetChar();

	/// Flush internal input buffer.
	void inputCharFlush();

	///
	void inputSetMouseResolution(uint16_t _width, uint16_t _height);

	///
	void inputSetMousePos(int32_t _mx, int32_t _my, int32_t _mz);

	///
	void inputSetMouseButtonState(vr::MouseButton::Enum _button, uint8_t _state);

	///
	void inputSetMouseLock(bool _lock);

	///
	void inputGetMouse(float _mouse[3]);

	///
	bool inputIsMouseLocked();

	///
	void inputSetGamepadAxis(vr::GamepadHandle _handle, vr::GamepadAxis::Enum _axis, int32_t _value);

	///
	int32_t inputGetGamepadAxis(vr::GamepadHandle _handle, vr::GamepadAxis::Enum _axis);

} // namespace vr