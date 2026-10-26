#pragma once

#include "RenderAPI.h"

#include <string>
#include <functional>
#include <vector>

namespace DRender
{

	class DRENDER_API RenderPass
	{
	public:
		RenderPass(const std::string& name);

		// Set the function to execute for this pass
		void SetExecute(std::function<void()> func);

		// Add resources that this pass reads and writes
		void AddReadResources(const std::string& read);
		void AddWriteResources(const std::string& write);

		// Ordering dependency independent of sampled resources.
		void AddDependency(const std::string& passName)
		{
			m_Dependencies.push_back(passName);
		}

		const std::vector<std::string>& GetDependencies() const
		{
			return m_Dependencies;
		}

		// Get the resources this pass reads
		const std::vector<std::string>& GetReadResources() const { return m_ReadResources; }
		// Get the resources this pass writes
		const std::vector<std::string>& GetWriteResources() const { return m_WriteResources; }
		
		// Execute the pass
		void Execute();

		const std::string& GetName() const { return m_Name; }
	private:
		std::string m_Name;

		std::function<void()> m_ExecuteFunc;

		std::vector<std::string> m_ReadResources;
		std::vector<std::string> m_WriteResources;
		std::vector<std::string> m_Dependencies;
	};

}
