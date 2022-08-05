#include "Program.h"
#include "Log.h"
#include "Display/Window.h"

Program* program;

int main()
{
	try
	{
		program = new Program();
		program->InitWindow();
	}
	catch (const DisplayInitException& exception)
	{
		Log::Error(exception.what());
	}
}