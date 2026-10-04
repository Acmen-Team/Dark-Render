#include "GLFrameBuffer.h"
#include "GLTexture2D.h"
#include <DTools.h>
#include <glad/glad.h>
#include <stdexcept>

namespace DRender
{
	namespace
	{
		void Allocate(const FrameBufferAttachment &attachment, uint32_t width, uint32_t height)
		{
			auto texture = attachment._Texture;
			texture->UploadTextureData(nullptr, width, height, 4);
			glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(reinterpret_cast<uintptr_t>(
											 texture->GetNativeHandle())));
			switch (attachment._Type)
			{
			case AttachmentType::eColor:
				break;
			case AttachmentType::eDepth:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
							 GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
				break;
			case AttachmentType::eStencil:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, width, height, 0,
							 GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
				break;
			case AttachmentType::eDepthStencil:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
							 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				break;
			}
			if (attachment._Type != AttachmentType::eColor)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
		}
		void Check()
		{
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw std::runtime_error("Incomplete scene framebuffer");
		}
	} // namespace
	GLFrameBuffer::~GLFrameBuffer()
	{
		if (m_FrameBufferID)
			glDeleteFramebuffers(1, &m_FrameBufferID);
	}
	void GLFrameBuffer::Initialize(const FrameBufferDesc &desc)
	{
		if (!desc._Width || !desc._Height)
			throw std::runtime_error("Invalid framebuffer size");
		if (m_FrameBufferID)
			glDeleteFramebuffers(1, &m_FrameBufferID);
		m_FrameBufferDesc = desc;
		glGenFramebuffers(1, &m_FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
		std::vector<GLenum> colors;
		for (auto &attachment : m_FrameBufferDesc._Attachments)
		{
			GLenum slot = GL_DEPTH_ATTACHMENT;
			switch (attachment._Type)
			{
			case AttachmentType::eColor:
				slot = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(colors.size());
				colors.push_back(slot);
				break;
			case AttachmentType::eDepth:
				break;
			case AttachmentType::eStencil:
				slot = GL_STENCIL_ATTACHMENT;
				break;
			case AttachmentType::eDepthStencil:
				slot = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			}
			if (!attachment._Texture)
			{
				attachment._Texture = CreateRef<GLTexture2D>();
				Allocate(attachment, desc._Width, desc._Height);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D,
								   static_cast<GLuint>(reinterpret_cast<uintptr_t>(
									   attachment._Texture->GetNativeHandle())),
								   0);
		}
		if (colors.empty())
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else
			glDrawBuffers(static_cast<GLsizei>(colors.size()), colors.data());
		Check();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void GLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
		glViewport(0, 0, m_FrameBufferDesc._Width, m_FrameBufferDesc._Height);
	}
	void GLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void GLFrameBuffer::Resize(int width, int height)
	{
		if (width <= 0 || height <= 0 || (width == GetWidth() && height == GetHeight()))
			return;
		m_FrameBufferDesc._Width = width;
		m_FrameBufferDesc._Height = height;
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
		for (auto &attachment : m_FrameBufferDesc._Attachments)
			Allocate(attachment, width, height);
		Check();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void GLFrameBuffer::Blit()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBufferID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, GetWidth(), GetHeight(), 0, 0, GetWidth(), GetHeight(),
						  GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	int GLFrameBuffer::GetWidth() const
	{
		return m_FrameBufferDesc._Width;
	}
	int GLFrameBuffer::GetHeight() const
	{
		return m_FrameBufferDesc._Height;
	}
	const FrameBufferDesc &GLFrameBuffer::GetDesc() const
	{
		return m_FrameBufferDesc;
	}
	bool GLFrameBuffer::ReadColorPixel(uint32_t x, uint32_t y, unsigned char rgba[4]) const
	{
		if (!rgba || x >= m_FrameBufferDesc._Width || y >= m_FrameBufferDesc._Height)
			return false;
		GLint previous = 0;
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBufferID);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
		const bool ok = glGetError() == GL_NO_ERROR;
		glBindFramebuffer(GL_READ_FRAMEBUFFER, previous);
		return ok;
	}
} // namespace DRender
