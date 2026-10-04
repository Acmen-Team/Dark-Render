#pragma once

#include "RenderAPI.h"

#include "IFrameBuffer.h"

namespace DRender
{

	class DRENDER_API FrameBufferFactory
	{
	public:
		// Creates a framebuffer with the specified description
		static Ref<IFrameBuffer> CreateFrameBuffer();
	};

}