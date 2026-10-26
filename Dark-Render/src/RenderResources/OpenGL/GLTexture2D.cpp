#include "GLTexture2D.h"

#include <glad/glad.h>
#include "../TextureMipmaps.h"
#include <cstring>

using namespace DResources;

namespace DRender
{
	GLTexture2D::~GLTexture2D()
	{
		if (m_TextureID)
		{
			glDeleteTextures(1, &m_TextureID);
		}
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
		if (width <= 0 || height <= 0 || (channels != 3 && channels != 4))
		{
			throw std::invalid_argument("Invalid OpenGL texture upload");
		}
		GLint maxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		if (width > maxTextureSize || height > maxTextureSize)
		{
			throw std::invalid_argument("Texture exceeds OpenGL image maxAnisotropy");
		}
		if (!m_TextureID)
		{
			glGenTextures(1, &m_TextureID);
		}
		m_Width = width;
		m_Height = height;
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		if (!pixelData)
		{
			// Framebuffer storage is a render target, not an imported mipmapped image.
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			return;
		}

		if (width <= 0 || height <= 0 || (channels != 3 && channels != 4))
		{
			throw std::invalid_argument("Invalid OpenGL texture upload");
		}
		std::vector<unsigned char> rgba(size_t(width) * height * 4);
		const auto* sourcePixels = static_cast<const unsigned char*>(pixelData);
		for (size_t i = 0; i < size_t(width) * height; ++i)
		{
			for (int channel = 0; channel < 3; ++channel)
			{
				rgba[i * 4 + channel] = sourcePixels[i * channels + channel];
			}
			rgba[i * 4 + 3] = channels == 4 ? sourcePixels[i * 4 + 3] : 255;
		}
		// The OpenGL renderer currently consumes color textures; PBR data maps use Vulkan.
		const auto chain =
			TextureMips::BuildMipChain(uint32_t(width), uint32_t(height), rgba, TextureMips::TextureMipKind::Color);
		for (size_t i = 0; i < chain._Levels.size(); ++i)
		{
			const auto& level = chain._Levels[i];
			glTexImage2D(GL_TEXTURE_2D, GLint(i), GL_RGBA8, GLsizei(level._Width), GLsizei(level._Height), 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, chain._Pixels.data() + level._Offset);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(chain._Levels.size() - 1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GLint majorVersion = 0, minorVersion = 0, extensionCount = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
		glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
		bool supportsAnisotropy = majorVersion > 4 || (majorVersion == 4 && minorVersion >= 6);
		for (GLint i = 0; i < extensionCount && !supportsAnisotropy; ++i)
		{
			const auto* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, GLuint(i)));
			supportsAnisotropy = extension && (!std::strcmp(extension, "GL_EXT_texture_filter_anisotropic") ||
											   !std::strcmp(extension, "GL_ARB_texture_filter_anisotropic"));
		}
		if (supportsAnisotropy)
		{
			GLfloat maxAnisotropy = 1;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(8.f, maxAnisotropy));
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
