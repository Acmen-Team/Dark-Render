#include "GLShader.h"

#include <glad/glad.h>

#include <DTools.h>

namespace DRender
{

	GLShader::GLShader()
	{
		if (m_ProgramID == 0)
		{
			m_ProgramID = glCreateProgram();
			if (m_ProgramID == 0)
			{
				DK_CORE_ERROR("Failed to create OpenGL shader program.");
			}
		}
	}

	GLShader::~GLShader()
	{
		if (m_ProgramID != 0)
		{
			glDeleteProgram(m_ProgramID);
			m_ProgramID = 0;
		}
	}

	std::vector<DResources::ShaderType> GLShader::GetShaderTypes() const
	{
		return m_ShaderTypes;
	}

	DResources::ShaderLanguage GLShader::GetShaderLanguage() const
	{
		return m_ShaderLanguage;
	}

	void GLShader::Compile()
	{
		glLinkProgram(m_ProgramID);
		GLint success;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
			DK_CORE_ERROR("Shader program linking failed: %s", infoLog);
			return;
		}
	}

	bool GLShader::IsCompiled() const
	{
		if (m_ProgramID == 0)
		{
			DK_CORE_ERROR("Shader program is not initialized.");
			return false;
		}

		GLint success;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			DK_CORE_ERROR("Shader program is not compiled successfully.");
			return false;
		}

		return true;
	}

	void GLShader::UploadUniformInt(const std::string& name, int value)
	{
		if (m_ProgramID == 0)
		{
			DK_CORE_ERROR("Shader program is not initialized.");
			return;
		}

		GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
		if (location == -1)
		{
			DK_CORE_ERROR("Uniform '%s' not found in shader program.", name.c_str());
			return;
		}

		glUseProgram(m_ProgramID);
		glUniform1i(location, value);
	}

	void GLShader::UploadUniformFloat(const std::string& name, float value)
	{
		if (m_ProgramID == 0)
		{
			DK_CORE_ERROR("Shader program is not initialized.");
			return;
		}

		GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
		if (location == -1)
		{
			DK_CORE_ERROR("Uniform '%s' not found in shader program.", name.c_str());
			return;
		}

		glUseProgram(m_ProgramID);
		glUniform1f(location, value);
	}

	void GLShader::UploadShaderSource()
	{
		for (const auto& shaderSource : m_ShaderSources)
		{
			if(shaderSource._Type == DResources::ShaderType::eVertex)
			{
				GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
				const char* source = shaderSource._SourceCode.c_str();
				glShaderSource(vertexShader, 1, &source, nullptr);
				glCompileShader(vertexShader);

				GLint success;
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					GLchar infoLog[512];
					glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
					DK_CORE_ERROR("Vertex shader compilation failed: %s", infoLog);
				}
				else
				{
					glAttachShader(m_ProgramID, vertexShader);
				}
				glDeleteShader(vertexShader);
			}
			else if (shaderSource._Type == DResources::ShaderType::eFragment)
			{
				GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
				const char* source = shaderSource._SourceCode.c_str();
				glShaderSource(fragmentShader, 1, &source, nullptr);
				glCompileShader(fragmentShader);

				GLint success;
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					GLchar infoLog[512];
					glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
					DK_CORE_ERROR("Fragment shader compilation failed: %s", infoLog);
				}
				else
				{
					glAttachShader(m_ProgramID, fragmentShader);
				}
				glDeleteShader(fragmentShader);
			}
			// Add other shader types as needed (geometry, compute, etc.)
			else
			{
				DK_CORE_ERROR("Unsupported shader type: %d", static_cast<int>(shaderSource._Type));
			}
		}
	}

}