#pragma once

#include <RenderResources/IBuffer.h>

namespace DRender
{

	class GLIndexBuffer : public IIndexBuffer
	{
	public:
		GLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~GLIndexBuffer();

		// Override methods from IBuffer
		virtual BufferType GetType() const override;
		virtual BufferMemoryType GetMemoryType() const override;

		// Override methods from IIndexBuffer
		virtual uint32_t GetCount() const override;

		// Binding
		virtual void Bind() const override;

	private:
		GLIndexBuffer(const GLIndexBuffer&) = delete;
		GLIndexBuffer& operator=(const GLIndexBuffer&) = delete;
		GLIndexBuffer(GLIndexBuffer&&) = delete;
		GLIndexBuffer& operator=(GLIndexBuffer&&) = delete;

	private:
		uint32_t m_BufferID{ 0 }; // OpenGL buffer ID
		uint32_t m_Count{ 0 };      // Number of indices
	};

}