#include "ForwardRenderPipeline.h"

#include "RenderBackend/ICommandBuffer.h"
#include "RenderGraph/RenderPass.h"

#include <DTools.h>
#include <Feature/IRenderFeature.h>

namespace DRender
{

	void ForwardRenderPipeline::Initialize(const RenderPipelineDesc& desc)
	{
		m_RenderGraph = desc._RenderGraph;
		m_RenderCommandBuffer = desc._RenderCommandBuffer;
		m_UseDepthPrepass = desc._UseDepthPrepass;

		if (m_RenderGraph == nullptr)
		{
			DK_CORE_ERROR("RenderGraph is null, cannot initialize render pipeline");
			return;
		}

		if (m_RenderCommandBuffer == nullptr)
		{
			DK_CORE_ERROR("RenderCommandBuffer is null, cannot initialize render pipeline");
			return;
		}

		// Additional initialization for forward rendering can be added here
		if (m_UseDepthPrepass)
		{
			RenderPass* depthPrepass = new RenderPass("DepthPrepass");

			//depthPrepass->SetExecute([]()
			//{
			//	// Depth pre-pass logic
			//	DK_CORE_INFO("Executing Depth Pre-pass");

			//});

		}
	}

	void ForwardRenderPipeline::RegisterRenderFeature(Ref<DFeature::IRenderFeature> feature)
	{
		if (feature == nullptr)
		{
			DK_CORE_ERROR("Cannot register a null render feature");
			return;
		}

		m_RenderFeatures.push_back(feature);
	}

	void ForwardRenderPipeline::BuildRenderGraph()
	{
		if (m_RenderGraph == nullptr)
		{
			DK_CORE_ERROR("RenderGraph is null, cannot build render graph");
			return;
		}

		DK_CORE_INFO("Building render graph with %zu features", m_RenderFeatures.size());

		for (const auto& feature : m_RenderFeatures)
		{
			if (feature)
			{
				feature->RegisterRenderPass(*m_RenderGraph);
			}
		}

		m_RenderGraph->BuildGraph();
		DK_CORE_INFO("Render graph built successfully");
	}

	void ForwardRenderPipeline::Execute()
	{
		if (m_RenderGraph == nullptr)
		{
			DK_CORE_ERROR("RenderGraph is null, cannot execute render graph");
			return;
		}

		DK_CORE_INFO("Executing render graph");

		m_RenderGraph->Execute();

		DK_CORE_INFO("Render graph executed successfully");
	}

}
