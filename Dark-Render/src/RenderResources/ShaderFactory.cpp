#include "RenderResources/ShaderFactory.h"

#include "RenderBackend/RenderBackendFactory.h"

#include "OpenGL/GLShader.h"

#include <DTools.h>

namespace DRender
{

	Ref<DResources::IShader> ShaderFactory::CreateShaderFromFile(const std::string& filePath)
	{
		if (filePath.empty())
		{
			DK_CORE_ERROR("ShaderFactory::CreateShaderFromFile: filePath is empty. Cannot create shader.");
			return nullptr;
		}

		Ref<GLShader> shader = nullptr;

		switch (RenderBackendFactory::GetRenderBackendType())
		{
			case RenderBackendType::OpenGL:
				shader = CreateRef<GLShader>();
				break;
			//case RenderBackendType::Vulkan:
			//	return new VulkanShader(filePath);
			default:
				DK_CORE_ERROR("ShaderFactory::CreateShaderFromFile: Unsupported render backend type.");
				return nullptr;
		}

		if (!shader->LoadFromFile(filePath))
		{
			DK_CORE_ERROR("ShaderFactory::CreateShaderFromFile: Failed to load shader from file: {}", filePath);
			shader.reset();
			return nullptr;
		}

		return shader;
	}

}