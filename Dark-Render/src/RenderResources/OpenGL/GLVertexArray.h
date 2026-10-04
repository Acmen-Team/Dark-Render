#pragma once

#include <RenderResources/IVertexArray.h>

namespace DRender
{

	class GLVertexArray : public IVertexArray
	{
	public:
		GLVertexArray();
		virtual ~GLVertexArray();

		// Override methods from IVertexArray
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(Ref<IVertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(Ref<IIndexBuffer> indexBuffer) override;

		virtual std::vector<Ref<IVertexBuffer>>& GetVertexBuffers() const override;
		virtual Ref<IIndexBuffer> GetIndexBuffer() const override;

	private:
		GLVertexArray(const GLVertexArray&) = delete;
		GLVertexArray& operator=(const GLVertexArray&) = delete;
		GLVertexArray(GLVertexArray&&) = delete;
		GLVertexArray& operator=(GLVertexArray&&) = delete;

	private:
		uint32_t m_ArrayID{ 0 }; // OpenGL Vertex Array Object ID
		std::vector<Ref<IVertexBuffer>> m_VertexBuffers; // List of vertex buffers
		Ref<IIndexBuffer> m_IndexBuffer{ nullptr }; // Pointer to the index buffer
	};

}