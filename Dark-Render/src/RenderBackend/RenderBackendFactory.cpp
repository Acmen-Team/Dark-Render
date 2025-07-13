#include "RenderBackend/RenderBackendFactory.h"

#include "OpenGL/GLRenderBackend.h"

#include <DTools.h>

namespace DRender
{

	RenderBackendType RenderBackendFactory::s_RenderBackendType;

	IRenderBackend* RenderBackendFactory::Create(RenderBackendType type, void* nativeWindowHandle)
	{
		if(nativeWindowHandle == nullptr)
		{
			DK_CORE_ERROR("RenderBackendFactory::Create: nativeWindowHandle is null. Cannot create render backend.");
		}

		s_RenderBackendType = type;

		switch (s_RenderBackendType)
		{
			case RenderBackendType::OpenGL:
				return new GLRenderBackend();
			//case RenderBackendType::Vulkan:
			//	return new VulkanRenderBackend(nativeWindowHandle);
			default:
				DK_CORE_ERROR("RenderBackendFactory::Create: Unsupported render backend type.");
				return nullptr;
		}
	}

	const char* RenderBackendFactory::GetRenderBackendName()
	{
		switch (s_RenderBackendType)
		{
			case RenderBackendType::OpenGL:
				return "OpenGL";
			//case RenderBackendType::Vulkan:
			//	return "Vulkan";
			default:
				DK_CORE_ERROR("RenderBackendFactory::GetRenderBackendName: Unsupported render backend type.");
				return "Unknown";
		}
	}

	RenderBackendType RenderBackendFactory::GetRenderBackendType()
	{
		return s_RenderBackendType;
	}

}