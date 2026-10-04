#include "RenderResources/FrameBufferFactory.h"

#include "OpenGL/GLFrameBuffer.h"

namespace DRender
{

	Ref<IFrameBuffer> FrameBufferFactory::CreateFrameBuffer()
	{
		return CreateRef<GLFrameBuffer>();
	}

} // namespace DRender