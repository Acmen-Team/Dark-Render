#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <queue>

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
		ResourceRegistry* m_ResourceRegistry = nullptr;

		// List of all render passes in the graph
		std::vector<RenderPass*> m_Passes;
		// Execution order of render passes
		std::vector<RenderPass*> m_ExecutionOrder;
		// Dependencies between resources
		std::unordered_map<std::string, std::vector<std::string>> m_ResourceDependencies;
	};

}