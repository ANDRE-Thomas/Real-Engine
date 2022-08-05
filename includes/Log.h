#pragma once

#include <string>
#include <iostream>
#include <windows.h>

class Log
{
public:
	static void Message(std::string message);
	static void Message(char* message);

	static void Warning(std::string message);
	static void Warning(char* message);

	static void Error(std::string message);
	static void Error(char* message);

private:
	static bool GetCurrentColor(WORD& returnValue);
};