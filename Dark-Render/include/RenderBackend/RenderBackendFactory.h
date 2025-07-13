#pragma once

#include "RenderAPI.h"

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
		static IRenderBackend* Create(RenderBackendType type, void* nativeWindowHandle);

		static const char* GetRenderBackendName();
		static RenderBackendType GetRenderBackendType();

	private:
		static RenderBackendType s_RenderBackendType;
	};
}