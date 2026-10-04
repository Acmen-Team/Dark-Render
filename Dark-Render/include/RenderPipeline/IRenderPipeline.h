#pragma once

#include "RenderFeature/IRenderFeature.h"
#include "RenderBackend/ICommandBuffer.h"

namespace DRender {

	struct RenderPipelineDesc
	{
		// Add any necessary fields for the render pipeline description
		Ref<RenderGraph> _RenderGraph = nullptr;
		Ref<ICommandBuffer> _RenderCommandBuffer = nullptr;
		bool _UseDepthPrepass = false;
	};

	class IRenderPipeline
	{
	public:
		virtual ~IRenderPipeline() = default;

		// Initialize the render pipeline
		virtual void Initialize(const RenderPipelineDesc& desc) = 0;

		virtual void RegisterRenderFeature(Ref<IRenderFeature> feature) = 0;

		virtual void BuildRenderGraph() = 0;

		virtual void Execute() = 0;

	};

}