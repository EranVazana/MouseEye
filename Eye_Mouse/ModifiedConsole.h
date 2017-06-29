#pragma once
#include <iostream>
#include <string>
#include <conio.h> //-For _getch.

#include <Windows.h>

//-Thread libaries.
#include <thread>
#include <chrono>

#include "Desktop.h"

//-Class used for working with the program console.
class ModifiedConsole {
public:
	ModifiedConsole();
	~ModifiedConsole();

	static void DestroyDefaultConsole();

	void Print(const char*);
	void Print(string);
	char WaitForKeyPress();

private:
	#define CONSOLE_TITLE "-MousEye Console-"
	#define CONSOLE_ICON_PATH "D:/Projects/Programming/Eye_Mouse/Eye_Mouse/Eye_Mouse/Media/ConsoleIcon.ico"
	#define CONSOLE_TEXT_COLOR 10 //-Matrix green color.

	#define KEY_PRESS_TEXT "\nPress any key to close the application."

	HANDLE _console_handle;
};