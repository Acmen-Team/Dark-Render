#pragma once

#include <Resources/IMesh.h>

#include "RenderResources/IVertexArray.h"

#include <Core/Core.h>

namespace DRender
{

	class GLMesh : public DResources::IMesh
	{
	public:
		GLMesh() = default;
		virtual ~GLMesh() = default;

		// Override methods from IMesh
		size_t GetSubMeshCount() const override;
		DResources::SubMesh* GetSubMesh(size_t index) const override;

		std::vector<Ref<IVertexArray>>& GetVertexArrays();

	protected:
		void UploadMeshData() override;

	private:
		// Prevent copying
		GLMesh(const GLMesh&) = delete;
		GLMesh& operator=(const GLMesh&) = delete;
		GLMesh(GLMesh&&) = delete;
		GLMesh& operator=(GLMesh&&) = delete;

	private:
		std::vector<Ref<IVertexArray>> m_VertexArrays; // List of vertex arrays for the mesh
	};

}