#pragma once

#include "RenderAPI.h"

#include <string>

namespace DResources {
	// Forward declaration for ITexture
	class IMesh;
}

namespace DRender
{

	class DRENDER_API MeshFactory
	{
	public:
		static DResources::IMesh* CreateMeshFromeFile(const std::string& filePath);
	};

}