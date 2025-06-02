#include "RenderGraph/RenderPass.h"

#include <DTools.h>

namespace DRender
{

	RenderPass::RenderPass(const std::string& name)
		: m_Name(name)
	{
		DK_CORE_INFO("RenderPass created: %s", name.c_str());
	}

	void RenderPass::SetExexute(std::function<void()> func)
	{
		m_ExecuteFunc = func;
	}

	void RenderPass::AddInput(const std::string& input)
	{
		m_Inputs.push_back(input);
	}

	void RenderPass::AddOutput(const std::string& output)
	{
		m_Outputs.push_back(output);
	}

	void RenderPass::Execute()
	{
		if (m_ExecuteFunc)
		{
			m_ExecuteFunc();
		}
		else
		{
			// Handle error: no execute function set
		}
	}

}