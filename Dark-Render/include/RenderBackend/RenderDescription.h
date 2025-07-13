#pragma once

namespace DRender
{

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
		uint32_t framebuffer = 0; // Placeholder for framebuffer ID

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

	struct ShaderProgram {
		uint32_t handle = 0; // OpenGL shader program ID
		// Vulkan 可以用 VkPipeline 或 pipeline layout 之类
	};

	struct PipelineDescription
	{
		// Define the structure for pipeline description
		// This could include shader stages, input layout, etc.
		// Shader program
		ShaderProgram shader;

		// 顶点输入布局（可选简化）
		std::vector<int> vertexAttributeLocations; // 例如位置 0 = position, 1 = uv, etc.

		// 状态设置
		bool depthTestEnable = true;
		bool depthWriteEnable = true;

		bool blendEnable = false;

		CullMode cullMode = CullMode::Back;
		FillMode fillMode = FillMode::Solid;
	};

}