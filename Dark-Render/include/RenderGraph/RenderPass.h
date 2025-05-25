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

		void SetExexute(std::function<void()> func);
		void AddInput(const std::string& input);
		void AddOutput(const std::string& output);

		void Execute();
	private:
		std::string m_Name;

		std::function<void()> m_ExecuteFunc;

		std::vector<std::string> m_Inputs;
		std::vector<std::string> m_Outputs;
	};

}