#pragma once

#include "RenderAPI.h"

#include <string>

namespace DResources {
	// Forward declaration for ITexture
	class ITexture;
}

namespace DRender
{

	class DRENDER_API TextureFactory
	{
	public:
		static DResources::ITexture* CreateTextureFromeFile(const std::string& filePath);

		static DResources::ITexture* CreateTextureFromeMemory(const void* pixelData, size_t size);
	};

}