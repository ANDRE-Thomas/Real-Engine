#include "Real Engine/Time.h"

#include <chrono>

double Time::deltaTime, Time::lastFrameTime;

double Time::GetDeltaTime()
{
	return deltaTime;
}

void Time::Update()
{
	double now = static_cast<double>(std::chrono::steady_clock::now().time_since_epoch().count()) / 1000000000.0;

	deltaTime = now - lastFrameTime;
	lastFrameTime = now;
}