#pragma once

static class Time
{
friend class Program;

public:
	static float GetDeltaTime();

private:
	static double deltaTime, lastFrameTime;

	static void Update();
};