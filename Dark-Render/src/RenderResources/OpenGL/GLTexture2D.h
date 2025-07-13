#pragma once

#include <Resources/ITexture.h>

namespace DRender
{

	class GLTexture2D : public DResources::ITexture
	{
	public:
		// Constructor for creating a texture from a file
		GLTexture2D() = default;
		virtual ~GLTexture2D() = default;

		// Override methods from ITexture
		virtual DResources::TextureFormat GetFormat() const override;
		virtual DResources::TextureType GetTextureType() const override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual unsigned char* GetPixelData() const override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual void* GetNativeHandle() const override;

	protected:
		virtual void UploadTextureData(const void* pixelData, int width, int height, int channels) override;

	private:
		// Prevent copying
		GLTexture2D(const GLTexture2D&) = delete;
		GLTexture2D& operator=(const GLTexture2D&) = delete;
		GLTexture2D(GLTexture2D&&) = delete;
		GLTexture2D& operator=(GLTexture2D&&) = delete;

	private:
		uint32_t m_TextureID{ 0 };  // OpenGL texture ID
	};

}