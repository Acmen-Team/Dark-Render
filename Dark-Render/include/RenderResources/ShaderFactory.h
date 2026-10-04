#pragma once

#include <Core/Core.h>

#include "RenderAPI.h"

#include <string>

namespace DResources
{
	class IShader;
}

namespace DRender
{

	class DRENDER_API ShaderFactory
	{
	public:
		// Creates a shader from a file path
		static Ref<DResources::IShader> CreateShaderFromFile(const std::string& filePath);
	};

}