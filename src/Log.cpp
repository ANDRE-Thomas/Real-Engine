#include "Log.h"

void Log::Message(std::string message)
{
	float previousColor = 15;
	GetCurrentColor(previousColor);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 15);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(console, previousColor);
}

void Log::Warning(std::string message)
{
	float previousColor = 15;
	GetCurrentColor(previousColor);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 14);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(console, previousColor);
}

void Log::Error(std::string message)
{
	float previousColor = 15;
	GetCurrentColor(previousColor);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 12);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(console, previousColor);
}

bool Log::GetCurrentColor(float& returnValue)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		return false;

	returnValue = info.wAttributes;
	return true;
}