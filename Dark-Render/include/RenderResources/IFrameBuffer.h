#pragma once

#include <vector>
#include <Core/Core.h>

namespace DResources
{
	class ITexture;
}

namespace DRender
{

	enum class AttachmentType
	{
		eColor,
		eDepth,
		eStencil,
		eDepthStencil
	};

	struct FrameBufferAttachment
	{
		AttachmentType _Type;
		Ref<DResources::ITexture> _Texture;
		bool _IsRenderTarget = true; // Default to true, indicating this attachment is a render target
	};

	struct FrameBufferDesc
	{
		uint32_t _Width;
		uint32_t _Height;
		std::vector<FrameBufferAttachment> _Attachments;
	};

	class IFrameBuffer
	{
	public:
		virtual ~IFrameBuffer() = default;

		virtual void Initialize(const FrameBufferDesc& desc) = 0;
		// Binding
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(int width, int height) = 0;

		// Blit
		virtual void Blit() = 0;

		// Getters
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual const FrameBufferDesc& GetDesc() const = 0;
		virtual bool ReadColorPixel(uint32_t x, uint32_t y, unsigned char rgba[4]) const = 0;
	};

}
