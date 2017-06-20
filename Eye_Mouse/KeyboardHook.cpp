#include "KeyboardHook.h"

//======================================================================
//-Since the callback function is a static methods, we use global variables.

HHOOK _hook;
KBDLLHOOKSTRUCT _kbd_struct;

//-Keys currently pressed by user.
CombinationKeys _current_pressed_keys;

//-We create a pointer to the current object so we could change the values from the static functions.
KeyboardHook* _pCurrent;

//======================================================================

//-Constructor.
KeyboardHook::KeyboardHook(CombinationKeys combination_keys, bool* flag, bool value_to_change){
	//Init the glocal KeyboardHook value to this object.
	_pCurrent = this;

	_combination_keys = combination_keys;
	_flag = flag;
	_value_to_change = value_to_change;
}

//-Destructor.
KeyboardHook::~KeyboardHook(){
	UnhookWindowsHookEx(_hook);
}

//-Start a thread for listeing to the keyboard.
void KeyboardHook::track() {
	thread listenThread(&KeyboardHook::listen, this);
	listenThread.detach();
}

//-Listen for the keyboard.
void KeyboardHook::listen() {
	//-Init the KeyboardHook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, NULL)))
		throw exception(HOOK_SETUP_ERROR);

	//-Listen for keyboard's input.
	LPMSG Msg{ NULL };
	while (GetMessage(Msg, NULL, 0, 0) > 0) {
		TranslateMessage(Msg);
		DispatchMessage(Msg);
	}
}

// Input: Boolean value.
//-Change the flag for the value of the function parameter.
void KeyboardHook::setFlag() {
	*_flag = _value_to_change;
}

//-Callback function.
LRESULT __stdcall KeyboardHook::HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		// lParam is the pointer to the struct containing the data needed, so cast and assign it to _kdb_struct.
		_kbd_struct = *((KBDLLHOOKSTRUCT*)lParam);

		//-Get the key value.
		OptionKey key = (OptionKey)(char)_kbd_struct.vkCode;

		if (wParam == WM_KEYDOWN)
			setKeyDown(key);
		else if (wParam == WM_KEYUP)
			setKeyUp(key);

		//-If the key is equal to the combination, change the flag value.
		if (_pCurrent->checkCombination()) {
			_pCurrent->setFlag();
			HOOK_KEYS; //-Don't pass the keys to the other programs.
		}
	}

	// call the next KeyboardHook in the KeyboardHook chain. This is nessecary or the KeyboardHook chain will break and the KeyboardHook stops.
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

// Input: New key.
// Output: Return true if the pressed keys list contain the key, false otherwise.
//-Check if the key is already in the pressed keys list.
bool KeyboardHook::isPressed(OptionKey key) {
	return find(_current_pressed_keys.begin(), _current_pressed_keys.end(), key) != _current_pressed_keys.end();
}

// Input: New key.
//-Insert the key to the pressed keys list.
void KeyboardHook::setKeyDown(OptionKey key) {
	if (!isPressed(key))
		_current_pressed_keys.push_back(key);
}

// Input: Pressed key.
//-Remove the key from the pressed keys list.
void KeyboardHook::setKeyUp(OptionKey key) {
	if (isPressed(key))
		_current_pressed_keys.remove(key);
}

//-Checks if the currents pressed keys are equal to the specified combination.
bool KeyboardHook::checkCombination() {
	return (_current_pressed_keys.size() == _combination_keys.size()) ? equal(_current_pressed_keys.begin(), _current_pressed_keys.end(), _combination_keys.begin()) : false;
}