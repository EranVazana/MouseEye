#pragma once
#include <iostream>
#include <string>
#include <conio.h> //-For _getch.

#include <Windows.h>

//-Thread libaries.
#include <thread>
#include <chrono>

using namespace std;

//-Namespace used for working with the program console.
namespace Console{
	void HideConsole();
	void ShowConsole();
	void HideConsoleCursor();

	void Print(const char*);
	void Print(string);
	char WaitForKeyPress();
};