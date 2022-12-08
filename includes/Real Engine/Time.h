#pragma once

static class Time
{
	friend class Program;

public:
	static double GetDeltaTime();

private:
	static double deltaTime, lastFrameTime;

	static void Update();
};