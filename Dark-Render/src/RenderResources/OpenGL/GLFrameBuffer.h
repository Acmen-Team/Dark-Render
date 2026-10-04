#pragma once

#include <Core/Core.h>
#include <Resources/ITexture.h>

#include <RenderResources/IFrameBuffer.h>

namespace DRender
{

	class GLFrameBuffer : public IFrameBuffer
	{
	public:
		GLFrameBuffer() = default;
		virtual ~GLFrameBuffer();

		// Override methods from IFrameBuffer
		virtual void Initialize(const FrameBufferDesc& desc) override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(int width, int height) override;

		virtual void Blit() override;

		// Getters
		virtual int GetWidth() const override;
		virtual int GetHeight() const override;
		virtual const FrameBufferDesc& GetDesc() const override;
		bool ReadColorPixel(uint32_t x, uint32_t y, unsigned char rgba[4]) const override;

	private:
		GLFrameBuffer(const GLFrameBuffer&) = delete;
		GLFrameBuffer& operator=(const GLFrameBuffer&) = delete;
		GLFrameBuffer(GLFrameBuffer&&) = delete;
		GLFrameBuffer& operator=(GLFrameBuffer&&) = delete;

	private:
		uint32_t m_FrameBufferID = 0; // OpenGL frame buffer ID
		FrameBufferDesc m_FrameBufferDesc;        // Frame buffer description
	};

}
