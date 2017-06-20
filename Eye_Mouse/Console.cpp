#include "Console.h"

//======================================================================

//-Hide the program's console.
void Console::HideConsole(){
	//-Delete the existing console.
	FreeConsole();
}

//-Show the programs's console.
void Console::ShowConsole(){
	//-Allocate new console and set new stream.
	AllocConsole();
}

//-Hide the console's cursor.
void Console::HideConsoleCursor(){
	//-Get the console's handle.
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	//-Get a pointer of the console's cursor.
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);

	//-Set the curosr to be non-visible.
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

// Input: String to print.
//-Print the string on the console.
void Console::Print(const char* s) {
	HANDLE myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD cCharsWritten;
	WriteConsole(myConsoleHandle, TEXT(s), strlen(s), &cCharsWritten, NULL);
}

// Input: Std::string.
//-Print the string on the console.
void Console::Print(string s) {
	Print(s.c_str());
}

// Output: returns the value of the pressed key.
//-Wait for a key press by the user.
char Console::WaitForKeyPress(){
	Print("\nPress any key to close the application.");
	return _getch();
}