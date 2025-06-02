#include "RenderGraph/RenderGraph.h"

#include "RenderGraph/RenderPass.h"
#include "RenderGraph/ResourceRegistry.h"

namespace DRender
{

	void RenderGraph::AddPass(RenderPass* pass)
	{
		m_Passes.push_back(pass);
	}

	void RenderGraph::BuildGraph()
	{
		TopologicalSort();
	}

	void RenderGraph::Execute()
	{
		for (auto pass : m_ExecutionOrder)
		{
			pass->Execute();
		}
	}

	void RenderGraph::SetResourceRegistry(ResourceRegistry* registry)
	{
		m_ResourceRegistry = registry;
	}

	void RenderGraph::TopologicalSort()
	{
		m_ExecutionOrder = m_Passes;
	}

}