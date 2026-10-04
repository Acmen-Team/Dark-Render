#include "GLTexture2D.h"

#include <glad/glad.h>

using namespace DResources;

namespace DRender
{
	GLTexture2D::~GLTexture2D()
	{
		if (m_TextureID) glDeleteTextures(1, &m_TextureID);
	}

	TextureFormat GLTexture2D::GetFormat() const
	{
		return m_Format;
	}

	TextureType GLTexture2D::GetTextureType() const
	{
		return TextureType::eTexture2D;
	}

	uint32_t GLTexture2D::GetWidth() const
	{
		return m_Width;
	}

	uint32_t GLTexture2D::GetHeight() const
	{
		return m_Height;
	}

	unsigned char* GLTexture2D::GetPixelData() const
	{
		return m_PixelData;
	}

	void GLTexture2D::Bind(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void GLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void* GLTexture2D::GetNativeHandle() const
	{
		return reinterpret_cast<void*>(static_cast<intptr_t>(m_TextureID));
	}

	void GLTexture2D::UploadTextureData(const void* pixelData, int width, int height, int channels)
	{
		if (!m_TextureID) glGenTextures(1, &m_TextureID);
		m_Width = width;
		m_Height = height;
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
