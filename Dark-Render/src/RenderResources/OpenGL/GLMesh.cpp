#include <stdexcept>
#include "GLMesh.h"

#include "GLIndexBuffer.h"
#include "GLVertexBuffer.h"
#include "GLVertexArray.h"

#include <DTools.h>

namespace DRender
{

	size_t GLMesh::GetSubMeshCount() const
	{
		return m_SubMeshList.size();
	}

	DResources::SubMesh* GLMesh::GetSubMesh(size_t index) const
	{
		return m_SubMeshList.at(index);
	}

	std::vector<Ref<IVertexArray>>& GLMesh::GetVertexArrays()
	{
		return m_VertexArrays;
	}

	void GLMesh::UploadMeshData()
	{
		std::vector<Ref<IVertexArray>> replacement;
		replacement.reserve(m_SubMeshList.size());
		Ref<GLVertexArray> vertexArray;
		Ref<GLVertexBuffer> vertexBuffer;
		Ref<GLIndexBuffer> indexBuffer;

		for (const auto& subMesh : m_SubMeshList)
		{
			if(!subMesh || subMesh->_Vertices.empty() || subMesh->_Indices.empty()) throw std::runtime_error("Cannot upload empty submesh");
			vertexArray.reset(new GLVertexArray());

			DK_CORE_INFO("GLMesh::UploadMeshData: Uploading submesh with %d vertices and %d indices",
				subMesh->_Vertices.size(), subMesh->_Indices.size());

			vertexBuffer.reset(new GLVertexBuffer((float*)subMesh->_Vertices.data(), sizeof(DResources::Vertex) * subMesh->_Vertices.size()));

			indexBuffer.reset(new GLIndexBuffer(subMesh->_Indices.data(), subMesh->_Indices.size()));

			vertexBuffer->SetLayout({
				{ DRender::ShaderDataType::Float3, "a_Position" },
				{ DRender::ShaderDataType::Float3, "a_Normal" },
				{ DRender::ShaderDataType::Float2, "a_TexCoord" }
			});

			vertexArray->AddVertexBuffer(vertexBuffer);
			vertexArray->SetIndexBuffer(indexBuffer);

			replacement.push_back(vertexArray);
		}
        m_VertexArrays.swap(replacement);
	}

}