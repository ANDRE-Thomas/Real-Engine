#include "Real Engine/Log.h"

#include <iostream>

void Log::Message(char* message)
{
	if (message == nullptr)
		return;

	Log::Message(std::string(message));
}

void Log::Message(std::string message)
{
	WORD previousColor = 15;
	GetCurrentColor(previousColor);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 15);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(console, previousColor);
}

void Log::Warning(char* message)
{
	if (message == nullptr)
		return;

	Log::Warning(std::string(message));
}

void Log::Warning(std::string message)
{
	WORD previousColor = 15;
	GetCurrentColor(previousColor);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 14);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(console, previousColor);
}

void Log::Error(char* message)
{
	if (message == nullptr)
		return;

	Log::Error(std::string(message));
}

void Log::Error(std::string message)
{
	WORD previousColor = 15;
	GetCurrentColor(previousColor);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 12);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(console, previousColor);
}

bool Log::GetCurrentColor(WORD& returnValue)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		return false;

	returnValue = info.wAttributes;
	return true;
}