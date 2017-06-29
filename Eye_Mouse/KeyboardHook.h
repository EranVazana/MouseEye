#pragma once
#include <Windows.h>
#include <thread>
#include <list>
#include <algorithm>

using namespace std;

//-All available key option for the hook.
enum OptionKey {
	L_SHIFT = -96, R_SHIFT, _LCTRL, R_CTRL, L_ALT, R_ALT,
	F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	ESC = 27
};

typedef list<OptionKey> CombinationKeys;

//-Low level keyboard hook used to capture keyboard input for click-through windows.
class KeyboardHook {
public:
	KeyboardHook(CombinationKeys, bool*, bool);
	~KeyboardHook();

	void track();

private:
	#define HOOK_KEYS { return -1; }

	void listen();
	void setFlag();

	static LRESULT __stdcall HookCallback(int, WPARAM, LPARAM);
	static bool isPressed(OptionKey);
	static void setKeyDown(OptionKey);
	static void setKeyUp(OptionKey);
	bool checkCombination();

	CombinationKeys _combination_keys;
	bool* _flag;
	bool _value_to_change;

	//======================================================================
	//-Custom exceptions:

	class HookSetupException : public exception {
		virtual const char* what() const throw() {
			return "--(!) An Error occurred while opening the Area Selection GUI.";
		}
	};
};