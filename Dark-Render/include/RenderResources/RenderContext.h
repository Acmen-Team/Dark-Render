#pragma once

namespace DRender {

	struct FrameContext
	{
		Ref<IMesh> _Mesh;
	};

	class RenderContext
	{
	public:

		void BindPipeline(const RenderPipelineDesc& desc);
		void Render()
	};


}