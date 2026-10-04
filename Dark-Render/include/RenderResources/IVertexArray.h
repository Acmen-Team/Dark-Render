#pragma once

#include "IBuffer.h"

#include <vector>

namespace DRender
{

	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		// Binding
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Vertex Buffer management
		virtual void AddVertexBuffer(Ref<IVertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(Ref<IIndexBuffer> indexBuffer) = 0;

		// Getters
		virtual std::vector<Ref<IVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual Ref<IIndexBuffer> GetIndexBuffer() const = 0;

	protected:
		IVertexArray() = default;
	};

}