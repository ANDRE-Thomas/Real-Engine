#pragma once

#include <string>
#include <iostream>
#include <windows.h>

static class Log
{
public:
	static void Message(std::string message);
	static void Warning(std::string message);
	static void Error(std::string message);

private:
	static bool GetCurrentColor(float& returnValue);
};