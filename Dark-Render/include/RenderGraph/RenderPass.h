#pragma once
#include <string>
#include <functional>
#include <vector>

#include <Macro/DynamicLinkMacros.h>

namespace DRender
{

	class DARK_API RenderPass
	{
	public:
		RenderPass(const std::string& name);

		// Set the function to execute for this pass
		void SetExexute(std::function<void()> func);

		// Add resources that this pass reads and writes
		void AddReadResources(const std::string& read);
		void AddWriteResources(const std::string& write);

		// Get the resources this pass reads
		const std::vector<std::string>& GetReadResources() const { return m_ReadResources; }
		// Get the resources this pass writes
		const std::vector<std::string>& GetWriteResources() const { return m_WriteResource; }
		
		// Execute the pass
		void Execute();

		const std::string& GetName() const { return m_Name; }
	private:
		std::string m_Name;

		std::function<void()> m_ExecuteFunc;

		std::vector<std::string> m_ReadResources;
		std::vector<std::string> m_WriteResource;
	};

}