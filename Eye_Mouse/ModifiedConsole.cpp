#include "ModifiedConsole.h"

//======================================================================

//-Constructor.
ModifiedConsole::ModifiedConsole() {
	AllocConsole();
	SetConsoleTitle(CONSOLE_TITLE);
	Desktop::SetIcon(GetConsoleWindow());

	_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(_console_handle, CONSOLE_TEXT_COLOR);
}

//-Destructor.
ModifiedConsole::~ModifiedConsole() {

}

//-Delete the existing console.
void ModifiedConsole::DestroyDefaultConsole() {
	FreeConsole();
}

// Input: String to print.
//-Print the string on the console.
void ModifiedConsole::Print(const char* str) {
	DWORD cCharsWritten;
	WriteConsole(_console_handle, TEXT(str), strlen(str), &cCharsWritten, NULL);
}

// Input: std::string.
//-Print the string on the console.
void ModifiedConsole::Print(string str) {
	Print(str.c_str());
}

// Output: Returns the value of the pressed key.
//-Wait for a key press by the user.
char ModifiedConsole::WaitForKeyPress() {
	Print(KEY_PRESS_TEXT);
	return _getch();
}