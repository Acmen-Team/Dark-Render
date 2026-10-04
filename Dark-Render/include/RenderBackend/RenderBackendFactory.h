#pragma once

#include "RenderAPI.h"
#include <Core/Core.h>

#include <string>

namespace DRender
{
	enum class RenderBackendType
	{
		OpenGL = 0,
		Vulkan,
	};

	class IRenderBackend;

	class DRENDER_API RenderBackendFactory
	{
	public:
		static Ref<IRenderBackend> Create(RenderBackendType type, void* nativeWindowHandle);

		static const char* GetRenderBackendName();
		static RenderBackendType GetRenderBackendType();

	private:
		static RenderBackendType s_RenderBackendType;
		static Ref<IRenderBackend> s_RenderBackendPtr;
	};
}
