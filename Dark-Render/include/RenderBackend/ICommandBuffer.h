#pragma once

#include "RenderAPI.h"

#include <vector>
#include <RenderBackend/RenderDescription.h>

namespace DRender
{

	class DRENDER_API ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void BeginRenderPass(const RenderPassDescription& rpDesc) = 0;
		virtual void EndRenderPass() = 0;

		virtual void BindPipeline(const PipelineDescription& plDesc) = 0;

		virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
	};

}