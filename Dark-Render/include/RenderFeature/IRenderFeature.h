#pragma once

#include "RenderGraph/RenderGraph.h"

namespace DRender {

	class IRenderFeature
	{
	public:
		virtual ~IRenderFeature() = default;

		virtual void RegisterRenderPass(RenderGraph& rg) = 0;

		virtual int GetPriority() const = 0;

		virtual std::string GetName() const = 0;
	};

}