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

	void RenderPass::AddReadResources(const std::string& read)
	{
		m_ReadResources.push_back(read);
	}

	void RenderPass::AddWriteResources(const std::string& write)
	{
		m_WriteResource.push_back(write);
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