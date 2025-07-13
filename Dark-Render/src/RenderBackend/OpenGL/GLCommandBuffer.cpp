#include "GLCommandBuffer.h"

#include <glad/glad.h>

namespace DRender
{

	void GLCommandBuffer::Begin()
	{

	}

	void GLCommandBuffer::End()
	{

	}

	void GLCommandBuffer::BeginRenderPass(const RenderPassDescription& rpDesc)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rpDesc.framebuffer);

		glViewport(0, 0, rpDesc.width, rpDesc.height);

		GLbitfield clearMask = 0;
		if (rpDesc.clearColor)
		{
			clearMask |= GL_COLOR_BUFFER_BIT;
			glClearColor(rpDesc.clearColorValue.r, rpDesc.clearColorValue.g, rpDesc.clearColorValue.b, rpDesc.clearColorValue.a);
		}
		if (rpDesc.clearDepth)
		{
			clearMask |= GL_DEPTH_BUFFER_BIT;
			glClearDepthf(rpDesc.clearDepthValue);
		}
		if (clearMask != 0)
		{
			glClear(clearMask);
		}

		if (rpDesc.enableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (rpDesc.enableBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Default blending function
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLCommandBuffer::EndRenderPass()
	{

	}

	void GLCommandBuffer::BindPipeline(const PipelineDescription& plDesc)
	{
		if (plDesc.shader.handle != 0)
		{
			glUseProgram(plDesc.shader.handle);
		}
		else
		{
			glUseProgram(0); // Unbind shader program
		}

		if (plDesc.depthTestEnable)
		{
			glEnable(GL_DEPTH_TEST);
			//glDepthFunc(plDesc.depthFunc);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		glDepthMask(plDesc.depthWriteEnable ? GL_TRUE : GL_FALSE);

		if (plDesc.blendEnable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Default blending function
		}
		else
		{
			glDisable(GL_BLEND);
		}

		switch (plDesc.cullMode)
		{
			case CullMode::None:
				glDisable(GL_CULL_FACE);
				break;
			case CullMode::Front:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;
			case CullMode::Back:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;
		}

		switch (plDesc.fillMode)
		{
			case FillMode::Solid:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case FillMode::Wireframe:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
		}
	}

	void GLCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount /*= 1*/, uint32_t firstVertex /*= 0*/, uint32_t firstInstance /*= 0*/)
	{
		if (instanceCount > 1)
		{
			glDrawArraysInstanced(GL_TRIANGLES, firstVertex, vertexCount, instanceCount);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, firstVertex, vertexCount);
		}
	}

}