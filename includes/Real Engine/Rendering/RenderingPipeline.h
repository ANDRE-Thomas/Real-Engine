#pragma once

#include "Real Engine/Rendering/IRenderingViewPoint.h"

class RenderingPipeline
{
public:
	RenderingPipeline();
	~RenderingPipeline();

	virtual void Init() = 0;

	virtual void Render(IRenderingViewPoint* viewPoint) = 0;
};