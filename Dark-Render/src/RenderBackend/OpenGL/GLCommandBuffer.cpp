#include "GLCommandBuffer.h"

#include <glad/glad.h>

#include "RenderResources/OpenGL/GLMesh.h"
#include "RenderResources/OpenGL/GLShader.h"
#include "RenderResources/OpenGL/GLFrameBuffer.h"
#include <Resources/IMaterial.h>

#include <DTools.h>

#include <memory>
#include <stdexcept>

using namespace DTools;

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
		//std::dynamic_pointer_cast<GLFrameBuffer>(rpDesc._Framebuffer.g)->Bind();
		//rpDesc._Framebuffer->Bind();
		std::dynamic_pointer_cast<GLFrameBuffer>(const_cast<std::shared_ptr<IFrameBuffer>&>(rpDesc._Framebuffer))->Bind();

		GLbitfield clearMask = 0;
		glDepthMask(GL_TRUE);
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
		plDesc._Material->Bind();

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

	void GLCommandBuffer::DrawMesh(DResources::IMesh* mesh)
    {
        if(!mesh) throw std::invalid_argument("Null mesh");
        for(size_t i=0;i<mesh->GetSubMeshCount();++i) DrawSubMesh(mesh,i);
    }
    void GLCommandBuffer::DrawSubMesh(DResources::IMesh* mesh,size_t index)
    {
        auto* glMesh=dynamic_cast<GLMesh*>(mesh);
        if(!glMesh || index>=glMesh->GetVertexArrays().size()) throw std::invalid_argument("Invalid OpenGL submesh");
        const auto& array=glMesh->GetVertexArrays().at(index);
        array->Bind(); glDrawElements(GL_TRIANGLES,array->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr); array->Unbind();
    }
}