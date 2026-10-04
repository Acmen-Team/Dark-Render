#pragma once

#include <cstdint>

#include <DTools.h>

namespace DRender
{

	class BufferLayout;

	enum class BufferType
	{
		eVertexBuffer,
		eIndexBuffer,
		eUniformBuffer,
		eStorageBuffer,
	};

	enum class BufferMemoryType
	{
		eGPUOnly,
		eCPUOnly,
		eCPUToGPU,
		eGPUToCPU,
	};

	//struct BufferDesc
	//{
	//	BufferType _Type;
	//	BufferMemoryType _MemoryType;
	//	size_t _Size; // Size in bytes
	//	void* _BufferData; // Pointer to initial data, can be nullptr
	//};

	class IBuffer
	{
	public:
		virtual ~IBuffer() = default;

		// Getters
		virtual BufferType GetType() const = 0;
		virtual BufferMemoryType GetMemoryType() const = 0;
	};

	class IVertexBuffer : public IBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		// Layout management
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		// Binding
		virtual void Bind() const = 0;

	protected:
		// Constructor is protected to prevent direct instantiation
		IVertexBuffer() = default;

	private:
		// Prevent copying and moving
		IVertexBuffer(const IVertexBuffer&) = delete;
		IVertexBuffer& operator=(const IVertexBuffer&) = delete;
		IVertexBuffer(IVertexBuffer&&) = delete;
		IVertexBuffer& operator=(IVertexBuffer&&) = delete;
	};

	class IIndexBuffer : public IBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		// Count of indices
		virtual uint32_t GetCount() const = 0;

		// Binding
		virtual void Bind() const = 0;

	protected:
		// Constructor is protected to prevent direct instantiation
		IIndexBuffer() = default;

	private:
		// Prevent copying and moving
		IIndexBuffer(const IIndexBuffer&) = delete;
		IIndexBuffer& operator=(const IIndexBuffer&) = delete;
		IIndexBuffer(IIndexBuffer&&) = delete;
		IIndexBuffer& operator=(IIndexBuffer&&) = delete;
	};

}