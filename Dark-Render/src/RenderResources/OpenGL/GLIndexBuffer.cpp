#include "GLIndexBuffer.h"

#include <glad/glad.h>
#include <DTools.h>

namespace DRender
{

	GLIndexBuffer::GLIndexBuffer(uint32_t* indices, uint32_t count)
	{
		DK_CORE_ASSERT(count > 0, "Index count must be greater than zero.");
		DK_CORE_ASSERT(indices != nullptr, "Indices pointer cannot be null.");

		m_Count = count;

		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		if (m_BufferID != 0)
		{
			glDeleteBuffers(1, &m_BufferID);
			m_BufferID = 0;
		}
	}

	BufferType GLIndexBuffer::GetType() const
	{
		return BufferType::eIndexBuffer;
	}

	BufferMemoryType GLIndexBuffer::GetMemoryType() const
	{
		return BufferMemoryType::eGPUOnly;
	}

	uint32_t GLIndexBuffer::GetCount() const
	{
		return m_Count;
	}

	void GLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

}