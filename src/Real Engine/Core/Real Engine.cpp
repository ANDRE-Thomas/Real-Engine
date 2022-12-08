#include "Real Engine/Core/Program.h"
#include "Real Engine/Core/Log.h"

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