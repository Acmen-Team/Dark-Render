#pragma once

#include <Core/Core.h>

namespace DResources
{
	class IMaterial;
}

namespace DRender
{

	class IFrameBuffer;

	struct ClearColor
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f; // Default to opaque white
	};

	struct RenderPassDescription
	{
		// Define the structure for render pass description
		// This could include color attachments, depth attachments, etc.
		Ref<IFrameBuffer> _Framebuffer = nullptr; // Placeholder for framebuffer ID

		uint32_t width = 0;
		uint32_t height = 0;

		bool clearColor = true;
		bool clearDepth = true;

		ClearColor clearColorValue;

		float clearDepthValue = 1.0f; // Default to 1.0 for depth buffer

		bool enableDepthTest = true; // Enable depth testing by default

		bool enableBlending = false; // Disable blending by default
	};

	enum class CullMode {
		None,
		Front,
		Back
	};

	enum class FillMode {
		Solid,
		Wireframe
	};

	enum class ShaderStage {
		Vertex,
		Fragment
	};

	struct PipelineDescription
	{
		Ref<DResources::IMaterial> _Material;

		// �������벼�֣���ѡ�򻯣�
		std::vector<int> vertexAttributeLocations; // ����λ�� 0 = position, 1 = uv, etc.

		// ״̬����
		bool depthTestEnable = true;
		bool depthWriteEnable = true;

		bool blendEnable = false;

		CullMode cullMode = CullMode::Back;
		FillMode fillMode = FillMode::Solid;
	};

}