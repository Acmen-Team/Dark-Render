#pragma once

#include "RenderAPI.h"
#include "RenderBackend/ICommandBuffer.h"

#include <string>
#include <Windows.h>

namespace DRender
{

	struct BackendInitInfo
	{
		void* _NativeWindowHandle = nullptr;
		void* _SharedContext = nullptr;
		void* _PlatformHandle = nullptr;
	};

	class DRENDER_API IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;

		virtual void Initialize(BackendInitInfo* initInfo) = 0;
		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void PushDebugGroup(const std::string& name) = 0;
		virtual void PopDebugGroup() = 0;

		virtual ICommandBuffer* CreateCommandBuffer() = 0;
		virtual void SubmitCommandBuffer(ICommandBuffer* commandBuffer) = 0;

		virtual std::string GetName() const = 0;
	};

}