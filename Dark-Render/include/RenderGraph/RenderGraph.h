#pragma once

#include <vector>
#include <unordered_map>

#include <Macro/DynamicLinkMacros.h>

namespace DRender
{

	// Forward declaration of RenderPass
	class RenderPass;
	class ResourceRegistry;

	class DARK_API RenderGraph
	{
	public:
		void AddPass(RenderPass* pass);

		void BuildGraph();

		void Execute();

		void SetResourceRegistry(ResourceRegistry* registry);

	private:
		std::vector<RenderPass*> m_Passes;
		std::vector<RenderPass*> m_ExecutionOrder;
		ResourceRegistry* m_ResourceRegistry = nullptr;

		void TopologicalSort();

	};

}