#pragma once

#include "RenderPipeline/IRenderPipeline.h"

namespace DRender
{

	class ForwardRenderPipeline : public IRenderPipeline
	{
	public:
		~ForwardRenderPipeline() override = default;

		void Initialize(const RenderPipelineDesc& desc) override;

		void RegisterRenderFeature(Ref<DFeature::IRenderFeature> feature) override;

		void BuildRenderGraph() override;

		void Execute() override;

	private:
		Ref<RenderGraph> m_RenderGraph = nullptr;
		Ref<ICommandBuffer> m_RenderCommandBuffer = nullptr;
		bool m_UseDepthPrepass = false;

		std::vector<Ref<DFeature::IRenderFeature>> m_RenderFeatures;
	};

}
