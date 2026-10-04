#pragma once

#include <Resources/IShader.h>

namespace DRender
{

	class GLShader : public DResources::IShader
	{
	public:
		GLShader();
		virtual ~GLShader();

		// Shader-specific methods
		std::vector<DResources::ShaderType> GetShaderTypes() const override;
		DResources::ShaderLanguage GetShaderLanguage() const override;

		void Compile() override;
		bool IsCompiled() const override;

		void UploadUniformInt(const std::string& name, int value) override;
		void UploadUniformFloat(const std::string& name, float value) override;

	protected:
		void UploadShaderSource() override;

	private:
				// Prevent copying
		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;
		GLShader(GLShader&&) = delete;
		GLShader& operator=(GLShader&&) = delete;

	private:
		unsigned int m_ProgramID = 0; // OpenGL program ID
	};

} // namespace DRender