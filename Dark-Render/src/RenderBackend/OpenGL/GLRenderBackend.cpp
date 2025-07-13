#include "GLRenderBackend.h"

#include "GLCommandBuffer.h"

#include <glad/glad.h>
#include <DTools.h>

#include <GL/gl.h>

#pragma comment(lib, "opengl32.lib")

namespace DRender
{
	static HDC s_OpenGLDC = nullptr; // Device Context
	static HGLRC s_OpenGLContext = nullptr;

	void GLRenderBackend::Initialize(BackendInitInfo* initInfo)
	{
		if(!initInfo || !initInfo->_NativeWindowHandle)
		{
			DK_CORE_ERROR("Failed to initialize OpenGL backend: initInfo or NativeWindowHandle is null.");
			return;
		}
		if (!initInfo->_SharedContext)
		{
			DK_CORE_ERROR("Failed to initialize OpenGL backend: HGLRC is null.");
			return;
		}

		HWND hwnd = static_cast<HWND>(initInfo->_NativeWindowHandle);
		HGLRC ctr = static_cast<HGLRC>(initInfo->_SharedContext);

		s_OpenGLDC = GetDC(hwnd);
		s_OpenGLContext = wglCreateContext(s_OpenGLDC);

		if (!s_OpenGLContext)
		{
			DK_CORE_ERROR("Failed to create OpenGL context.");
			return;
		}
		if (!wglShareLists(ctr, s_OpenGLContext))
		{
			DK_CORE_ERROR("Failed to share OpenGL context.");
			return;
		}

		wglMakeCurrent(s_OpenGLDC, s_OpenGLContext);

		if (!gladLoadGL())
		{
			DK_CORE_ERROR("Failed to initialize GLAD.");
			return;
		}
		gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
	}

	void GLRenderBackend::Shutdown()
	{
		// Cleanup OpenGL resources here
	}

	void GLRenderBackend::BeginFrame()
	{
		wglMakeCurrent(s_OpenGLDC, s_OpenGLContext);
		// Prepare for rendering a new frame
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderBackend::EndFrame()
	{
		// Finalize rendering and swap buffers
	}

	void GLRenderBackend::PushDebugGroup(const std::string& name)
	{
		// Push a debug group for better debugging and profiling
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name.c_str());
		DK_CORE_INFO("Pushed debug group: %s", name.c_str());

	}

	void GLRenderBackend::PopDebugGroup()
	{
		// Pop the last debug group
		glPopDebugGroup();
		DK_CORE_INFO("Popped debug group");
	}

	ICommandBuffer* GLRenderBackend::CreateCommandBuffer()
	{
		return new GLCommandBuffer();
	}

	void GLRenderBackend::SubmitCommandBuffer(ICommandBuffer* commandBuffer)
	{
		// Submit the command buffer to the OpenGL context for execution
	}

}