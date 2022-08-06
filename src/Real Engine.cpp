#include "Program.h"
#include "Log.h"
#include "Display/Window.h"

Program* program;

int main()
{
	try
	{
		program = new Program();
		program->InitGraph();
	}
	catch (const GraphInitException& exception)
	{
		Log::Error(exception.what());
	}	

	try
	{
		program->StartLoop();
	}
	catch (const std::runtime_error& error)
	{
		Log::Error(error.what());
	}
}