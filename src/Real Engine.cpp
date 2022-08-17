#include "Program.h"
#include "Log.h"
#include "Display/Window.h"

Program* program;

int main()
{
	try
	{
		Program::Init();
	}
	catch (const GraphInitException& exception)
	{
		Log::Error(exception.what());
	}	

	try
	{
		Program::StartLoop();
	}
	catch (const std::runtime_error& error)
	{
		Log::Error(error.what());
	}
}