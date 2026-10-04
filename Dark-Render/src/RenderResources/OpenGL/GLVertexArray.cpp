#include "GLVertexArray.h"

#include <glad/glad.h>
#include <DTools.h>

#include "GLVertexBuffer.h"

namespace DRender
{

	static GLenum ShaderDataTypeToOPenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	  return GL_FLOAT;
		case ShaderDataType::Float2:  return GL_FLOAT;
		case ShaderDataType::Float3:  return GL_FLOAT;
		case ShaderDataType::Float4:  return GL_FLOAT;
		case ShaderDataType::Mat3:	  return GL_FLOAT;
		case ShaderDataType::Mat4:	  return GL_FLOAT;
		case ShaderDataType::Int:			return GL_INT;
		case ShaderDataType::Int2:	  return GL_INT;
		case ShaderDataType::Int3:	  return GL_INT;
		case ShaderDataType::Int4:	  return GL_INT;
		case ShaderDataType::Bool:	  return GL_BOOL;
		}

		DK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	GLVertexArray::GLVertexArray()
	{
		glGenVertexArrays(1, &m_ArrayID);
	}

	GLVertexArray::~GLVertexArray()
	{
		if (m_ArrayID != 0)
		{
			glDeleteVertexArrays(1, &m_ArrayID);
			m_ArrayID = 0;
		}

		m_VertexBuffers.clear();
		m_IndexBuffer.reset();
	}

	void GLVertexArray::Bind() const
	{
		glBindVertexArray(m_ArrayID);
	}

	void GLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void GLVertexArray::AddVertexBuffer(Ref<IVertexBuffer> vertexBuffer)
	{
		DK_CORE_ASSERT(vertexBuffer->GetType() == BufferType::eVertexBuffer, "Invalid buffer type for vertex buffer.");
		DK_CORE_ASSERT(vertexBuffer->GetMemoryType() == BufferMemoryType::eGPUOnly, "Vertex buffer must be GPU only.");

		glBindVertexArray(m_ArrayID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout.GetElements())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOPenGLBaseType(element._Type),
				element._Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(element._Offset)
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}

	void GLVertexArray::SetIndexBuffer(Ref<IIndexBuffer> indexBuffer)
	{
		DK_CORE_ASSERT(indexBuffer->GetType() == BufferType::eIndexBuffer, "Invalid buffer type for index buffer.");
		DK_CORE_ASSERT(indexBuffer->GetMemoryType() == BufferMemoryType::eGPUOnly, "Index buffer must be GPU only.");

		glBindVertexArray(m_ArrayID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
		glBindVertexArray(0);
	}

	std::vector<Ref<IVertexBuffer>>& GLVertexArray::GetVertexBuffers() const
	{
		return const_cast<std::vector<Ref<IVertexBuffer>>&>(m_VertexBuffers);
	}

	Ref<IIndexBuffer> GLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

}