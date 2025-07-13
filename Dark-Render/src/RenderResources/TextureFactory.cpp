#include "RenderResources/TextureFactory.h"

#include "RenderBackend/RenderBackendFactory.h"

#include "OpenGL/GLTexture2D.h"

#include <DTools.h>
#include <Resources/ITexture.h>

using namespace DTools;

namespace DRender
{

	DResources::ITexture* TextureFactory::CreateTextureFromeFile(const std::string& filePath)
	{
		if (filePath.empty())
		{
			DK_CORE_ERROR("TextureFactory::CreateTextureFromeFile: filePath is empty. Cannot create texture.");
			return nullptr;
		}

		DResources::ITexture* texture = nullptr;

		switch (RenderBackendFactory::GetRenderBackendType())
		{
			case RenderBackendType::OpenGL:
				texture = new GLTexture2D();
				break;
			//case RenderBackendType::Vulkan:
			//	return new VulkanTexture2D(filePath);
			default:
				DK_CORE_ERROR("TextureFactory::CreateTextureFromeFile: Unsupported render backend type.");
				return nullptr;
		}

		if (!texture->LoadFromFile(filePath))
		{
			DK_CORE_ERROR("TextureFactory::CreateTextureFromeFile: Failed to load texture from file: {}", filePath);
			delete texture;
			return nullptr;
		}

		return texture;
	}

	DResources::ITexture* TextureFactory::CreateTextureFromeMemory(const void* pixelData, size_t size)
	{
		if (pixelData == nullptr || size == 0)
		{
			DK_CORE_ERROR("TextureFactory::CreateTextureFromeMemory: pixelData is null or size is zero. Cannot create texture.");
			return nullptr;
		}

		DResources::ITexture* texture = nullptr;

		switch (RenderBackendFactory::GetRenderBackendType())
		{
			case RenderBackendType::OpenGL:
				texture = new GLTexture2D();
				break;
			//case RenderBackendType::Vulkan:
			//	return new VulkanTexture2D(pixelData, size);
			default:
				DK_CORE_ERROR("TextureFactory::CreateTextureFromeMemory: Unsupported render backend type.");
				return nullptr;
		}

		if (!texture->LoadFromMemory(pixelData, size))
		{
			DK_CORE_ERROR("TextureFactory::CreateTextureFromeMemory: Failed to load texture from memory.");
			delete texture;
			return nullptr;
		}

		return texture;
	}

}