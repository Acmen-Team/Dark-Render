#pragma once

#include <RenderResources/IBuffer.h>

#include <string>
#include <vector>

namespace DRender
{

	enum class ShaderDataType
	{
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	uint32_t ShaderDataTypeSize(ShaderDataType type);

	struct BufferElement
	{
		std::string _Name;
		ShaderDataType _Type;
		uint32_t _Size;
		uint32_t _Offset;
		bool _Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: _Name(name), _Type(type), _Size(ShaderDataTypeSize(type)), _Offset(0), _Normalized(normalized) {}

		uint32_t GetComponentCount() const
		{
			switch (_Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Mat3:    return 3 * 3; // Assuming column-major order
			case ShaderDataType::Mat4:    return 4 * 4; // Assuming column-major order
			case ShaderDataType::Bool:    return 1;
			}
			return 0; // Default case, should never be reached
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements);

		const std::vector<BufferElement>& GetElements() const;
		uint32_t GetStride() const;

		void AddElement(const BufferElement& element);
		void Clear();

	private:
		void CalculateOffsetsAndStride();

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0; // Total size of one element in bytes
	};

	class GLVertexBuffer : public IVertexBuffer
	{
	public:
		GLVertexBuffer(float* vertices, uint32_t size);
		virtual ~GLVertexBuffer();

		// Override methods from IBuffer
		virtual BufferType GetType() const override;
		virtual BufferMemoryType GetMemoryType() const override;

		// Override methods from IVertexBuffer
		virtual const BufferLayout& GetLayout() const override;
		virtual void SetLayout(const BufferLayout& layout) override;
		virtual void Bind() const override;

	private:
		// Prevent copying
		GLVertexBuffer(const GLVertexBuffer&) = delete;
		GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;
		GLVertexBuffer(GLVertexBuffer&&) = delete;
		GLVertexBuffer& operator=(GLVertexBuffer&&) = delete;

	private:
		uint32_t m_BufferID{ 0 }; // OpenGL buffer ID
		BufferLayout m_Layout; // Layout of the vertex buffer
	};

}