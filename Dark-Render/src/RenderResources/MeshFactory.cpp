#include <memory>
#include "RenderResources/MeshFactory.h"

#include "RenderBackend/RenderBackendFactory.h"

#include "OpenGL/GLMesh.h"

#include <DTools.h>
#include <Resources/IMesh.h>

using namespace DTools;

namespace DRender
{

	DResources::IMesh* MeshFactory::CreateMeshFromeFile(const std::string& filePath)
	{
		if (filePath.empty())
		{
			DK_CORE_ERROR("MeshFactory::CreateMeshFromeFile: filePath is empty. Cannot create mesh.");
			return nullptr;
		}

		std::unique_ptr<GLMesh> mesh;

		switch (RenderBackendFactory::GetRenderBackendType())
		{
			case RenderBackendType::OpenGL:
				mesh=std::make_unique<GLMesh>();
				break;
			//case RenderBackendType::Vulkan:
			//	return new VulkanMesh(filePath);
			default:
				DK_CORE_ERROR("MeshFactory::CreateMeshFromeFile: Unsupported render backend type.");
				return nullptr;
		}

		if (!mesh->LoadFromFile(filePath))
		{
			DK_CORE_ERROR("MeshFactory::CreateMeshFromeFile: Failed to load mesh from file: {}", filePath);
			return nullptr;
		}

		return mesh.release();
	}

}