#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <DRender.h>

#include <iostream>
#include <windows.h>
/*
* This is a simple example of using the DRender library to create a triangle
*/

using namespace DRender;

// Define the vertices for a triangle
float vertices[] = {
		 0.0f,  0.5f, 0.0f, // top
		-0.5f, -0.5f, 0.0f, // left
		 0.5f, -0.5f, 0.0f  // right
};

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";

// Fragment shader source code
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Dark-Render Example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize the DRender library, this will also initialize the OpenGL context
	IRenderBackend* glRenderBackend = RenderBackendFactory::Create(RenderBackendType::OpenGL, glfwGetWin32Window(window));
	if (!glRenderBackend)
	{
		std::cerr << "Failed to create OpenGL render backend." << std::endl;
		return -1;
	}

	// 获取 HWND 和 HDC
	HWND hwnd = glfwGetWin32Window(window);      // 需要包含 <GLFW/glfw3native.h>
	// 获取主上下文句柄
	HGLRC mainCtx = wglGetCurrentContext();

	DRender::BackendInitInfo initInfo;
	initInfo._NativeWindowHandle = static_cast<void*>(hwnd);
	initInfo._SharedContext = static_cast<void*>(mainCtx);
	initInfo._PlatformHandle = nullptr;

	glRenderBackend->Initialize(&initInfo);

	// Create a render graph
	RenderGraph renderGraph;

	// Create a render pass
	RenderPass renderPass("TrianglePass");
	// Create a command buffer for this render pass
	ICommandBuffer* glCommandBuffer = glRenderBackend->CreateCommandBuffer();

	RenderPassDescription passDesc;
	passDesc.framebuffer = 0; // Use default framebuffer
	passDesc.width = 640;
	passDesc.height = 480;

	passDesc.clearColor = true;
	passDesc.clearColorValue = { 0.4f, 0.6f, 0.1f, 1.0f }; // Clear to black

	passDesc.clearDepth = true;
	passDesc.clearDepthValue = 1.0f; // Clear depth to 1.0

	passDesc.enableDepthTest = true; // Enable depth testing
	passDesc.enableBlending = false; // Disable blending

	PipelineDescription pipelineDesc;
	//pipelineDesc.shader.handle = glRenderBackend->CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
	pipelineDesc.shader.handle = 0; // Placeholder, replace with actual shader program creation
	pipelineDesc.depthTestEnable = true;
	pipelineDesc.depthWriteEnable = true;
	pipelineDesc.blendEnable = false;
	pipelineDesc.vertexAttributeLocations = { 0 }; // Assuming a single vertex attribute at location 0
	pipelineDesc.cullMode = CullMode::Back; // Default cull mode
	pipelineDesc.fillMode = FillMode::Solid; // Default fill mode

	renderPass.SetExecute([&]() {
		glCommandBuffer->Begin();

		glCommandBuffer->BeginRenderPass(passDesc);
		glCommandBuffer->BindPipeline(pipelineDesc);
		//glCommandBuffer->Draw();
		glCommandBuffer->EndRenderPass();

		glCommandBuffer->End();
	});

	renderGraph.AddPass(&renderPass);
	renderGraph.BuildGraph();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glRenderBackend->BeginFrame();

		/* Render here */
		/* Use the DRender library to draw something */
		renderGraph.Execute();

		glRenderBackend->EndFrame();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}