#include "Program.h"
#include "Log.h"
#include "Display/Window.h"

#include <glm/gtx/string_cast.hpp>

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

	program->camera->transform->SetPosition(vec3(1, 0, 10));

	Log::Message(to_string(program->camera->GetRenderMatrix()));
}