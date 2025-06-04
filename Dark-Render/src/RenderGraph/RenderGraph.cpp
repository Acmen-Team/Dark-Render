#include "RenderGraph/RenderGraph.h"

#include "RenderGraph/RenderPass.h"
#include "RenderGraph/ResourceRegistry.h"

#include <DTools.h>

using namespace DTools;

namespace DRender
{

	void RenderGraph::AddPass(RenderPass* pass)
	{
		m_Passes.push_back(pass);
	}

	void RenderGraph::BuildGraph()
	{
		std::unordered_map<std::string, std::vector<std::string>> resourceWriters;
		for (auto pass : m_Passes)
		{
			for (const auto& write : pass->GetWriteResources())
			{
				resourceWriters[write].push_back(pass->GetName());
			}
		}

		m_ResourceDependencies.clear();
		for (auto pass : m_Passes)
		{
			const std::string passnName = pass->GetName();
			m_ResourceDependencies[passnName] = std::vector<std::string>();

			for (const auto& read : pass->GetReadResources())
			{
				if (resourceWriters.find(read) != resourceWriters.end())
				{
					// Add dependencies for this resource
					for (const auto& writer : resourceWriters[read])
					{
						if (writer != passnName) // Avoid self-dependency
						{
							auto &dependencies = m_ResourceDependencies[passnName];
							if (std::find(dependencies.begin(), dependencies.end(), writer) == dependencies.end())
							{
								dependencies.push_back(writer);
							}
						}
					}
				}
			}
		}

		// Topological sort to determine execution order
		std::unordered_map<std::string, int> inDegree;
		for (const auto& pass : m_Passes)
		{
			inDegree[pass->GetName()] = 0; // Initialize in-degree
		}

		// Calculate in-degrees
		for (const auto& pair : m_ResourceDependencies)
		{
			for (const auto& dep : pair.second)
			{
				inDegree[pair.first]++; // Increment in-degree for each dependency
			}
		}

		std::queue<std::string> zeroInDegreeQueue;
		for (const auto& pair : inDegree)
		{
			if (pair.second == 0)
			{
				zeroInDegreeQueue.push(pair.first); // Add passes with zero in-degree
			}
		}

		// Create a map to quickly access passes by name
		std::unordered_map<std::string, RenderPass*> passMap;
		for (auto pass : m_Passes)
		{
			passMap[pass->GetName()] = pass; // Map pass names to pass objects
		}

		m_ExecutionOrder.clear();
		while (!zeroInDegreeQueue.empty())
		{
			std::string currentPassName = zeroInDegreeQueue.front();
			zeroInDegreeQueue.pop();

			RenderPass* currentPass = passMap[currentPassName];
			m_ExecutionOrder.push_back(currentPass); // Add to execution order

			// Decrease in-degree of dependent passes
			for (auto& pair : m_ResourceDependencies)
			{
				auto& dependencies = pair.second;
				if(std::find(dependencies.begin(), dependencies.end(), currentPassName) != dependencies.end())
				{
					// Remove the current pass from dependencies
					dependencies.erase(std::remove(dependencies.begin(), dependencies.end(), currentPassName), dependencies.end());

					// Decrease in-degree for this pass
					inDegree[pair.first]--;

					// If in-degree becomes zero, add to queue
					if (inDegree[pair.first] == 0)
					{
						zeroInDegreeQueue.push(pair.first);
					}
				}
			}
		}

		if (m_ExecutionOrder.size() != m_Passes.size())
		{
			DK_CORE_ERROR("RenderGraph: Cycle detected in render passes. Execution order incomplete.");
		}
		else
		{
			DK_CORE_INFO("RenderGraph built successfully with %zu passes.", m_ExecutionOrder.size());
			// Log execution order
			DK_CORE_INFO("Execution Order Render Graph:");
			for (const auto& pass : m_ExecutionOrder)
			{
				DK_CORE_INFO(" - %s", pass->GetName().c_str());
			}
		}

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

}