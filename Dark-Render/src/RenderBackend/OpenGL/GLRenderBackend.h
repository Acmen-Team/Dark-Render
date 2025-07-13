#pragma once

#include "RenderBackend/IRenderBackend.h"
#include "RenderBackend/ICommandBuffer.h"

#include <Windows.h>

namespace DRender
{

	class GLRenderBackend : public IRenderBackend
	{
	public:
		GLRenderBackend() = default;
		virtual ~GLRenderBackend() = default;

		void Initialize(BackendInitInfo* initInfo) override;
		void Shutdown() override;

		void BeginFrame() override;
		void EndFrame() override;

		void PushDebugGroup(const std::string& name) override;
		void PopDebugGroup() override;

		ICommandBuffer* CreateCommandBuffer() override;
		void SubmitCommandBuffer(ICommandBuffer* commandBuffer) override;

		std::string GetName() const override { return "OpenGL"; }
	};

}