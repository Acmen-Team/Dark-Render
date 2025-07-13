#pragma once

#include "RenderBackend/ICommandBuffer.h"

namespace DRender
{

	class GLCommandBuffer : public ICommandBuffer
	{
	public:
		GLCommandBuffer() = default;
		virtual ~GLCommandBuffer() = default;

		void Begin() override;
		void End() override;

		void BeginRenderPass(const RenderPassDescription& rpDesc) override;
		void EndRenderPass() override;

		void BindPipeline(const PipelineDescription& plDesc) override;

		void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0) override;
	};

}