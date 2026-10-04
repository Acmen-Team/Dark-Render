#include "GLVertexBuffer.h"


#include <glad/glad.h>

#include <DTools.h>

namespace DRender
{

	uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Float2:  return 4 * 2;
			case ShaderDataType::Float3:  return 4 * 3;
			case ShaderDataType::Float4:  return 4 * 4;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::Int2:    return 4 * 2;
			case ShaderDataType::Int3:    return 4 * 3;
			case ShaderDataType::Int4:    return 4 * 4;
			case ShaderDataType::Mat3:    return 4 * 3 * 3; // Assuming column-major order
			case ShaderDataType::Mat4:    return 4 * 4 * 4; // Assuming column-major order
			case ShaderDataType::Bool:    return 1;
		}

		DK_CORE_ASSERT(false, "Unknown ShaderDataType: %d", static_cast<int>(type));
		return 0; // Default case, should never be reached
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
	{
		m_Elements = elements;
		CalculateOffsetsAndStride();
		DK_CORE_ASSERT(!m_Elements.empty(), "BufferLayout must have at least one element.");
	}

	const std::vector<DRender::BufferElement>& BufferLayout::GetElements() const
	{
		return m_Elements;
	}

	uint32_t BufferLayout::GetStride() const
	{
		return m_Stride;
	}

	void BufferLayout::AddElement(const BufferElement& element)
	{
		DK_CORE_ASSERT(!element._Name.empty(), "BufferElement name cannot be empty.");
		DK_CORE_ASSERT(element._Size > 0, "BufferElement size must be greater than zero.");
		m_Elements.push_back(element);
		CalculateOffsetsAndStride();

	}

	void BufferLayout::Clear()
	{
		m_Elements.clear();
		m_Stride = 0;
		DK_CORE_ASSERT(m_Elements.empty(), "BufferLayout should be empty after clearing.");
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		m_Stride = 0;
		uint32_t offset = 0;

		for (auto& element : m_Elements)
		{
			element._Offset = offset;
			offset += element._Size;
			m_Stride += element._Size;
		}

		DK_CORE_ASSERT(m_Stride > 0, "BufferLayout stride must be greater than zero.");
	}

	GLVertexBuffer::GLVertexBuffer(float* vertices, uint32_t size)
	{
		DK_CORE_ASSERT(vertices != nullptr, "Vertex data cannot be null.");
		DK_CORE_ASSERT(size > 0, "Size must be greater than zero.");

		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		DK_CORE_ASSERT(m_BufferID != 0, "Failed to create OpenGL Vertex Buffer.");
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		if (m_BufferID != 0)
		{
			glDeleteBuffers(1, &m_BufferID);
			m_BufferID = 0;
		}
	}

	BufferType GLVertexBuffer::GetType() const
	{
		return BufferType::eVertexBuffer;
	}

	BufferMemoryType GLVertexBuffer::GetMemoryType() const
	{
		return BufferMemoryType::eGPUOnly; // Assuming GPU-only memory for vertex buffers
	}

	const BufferLayout& GLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void GLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		DK_CORE_ASSERT(!layout.GetElements().empty(), "BufferLayout must have at least one element.");
		m_Layout = layout;
	}

	void GLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

}