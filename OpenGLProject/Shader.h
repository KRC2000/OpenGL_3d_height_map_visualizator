#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace fwork
{
	class Shader
	{
		const char* defaultVert = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec2 aTexCoords;\n"
			"uniform mat4 model_mat4 = mat4(1);\n"
			"uniform mat4 view_mat4 = mat4(1);\n"
			"uniform mat4 projection_mat4 = mat4(1);\n"
			"out vec3 fragPos;\n"
			"out vec2 texCoords;\n"
			"void main()\n"
			"{\n"
			"   mat4 matrix = projection_mat4 * view_mat4 * model_mat4;\n"
			"   fragPos = vec3(aPos.x, aPos.y, aPos.z);\n"
			"	texCoords = aTexCoords;\n"
			"   gl_Position = matrix * vec4(aPos.x + gl_InstanceID*2, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		const char* defaultFrag = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform bool textured = false;\n"
			"uniform sampler2D tex;\n"
			"in vec3 fragPos;\n"
			"in vec2 texCoords;\n"
			"void main()\n"
			"{\n"
			"	if (textured)\n"
			"	{\n"
			"		vec4 color = texture(tex, texCoords);\n"
			"		if (color.a == 0) discard;\n"
			"		FragColor = color; \n"
			"	}\n"
			"	if (!textured) FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
			"}\n\0";
	public:
		unsigned int ID;
		// constructor generates the shader on the fly
		// ------------------------------------------------------------------------
		Shader(const char* vertexPath = nullptr, const char* fragmentPath = nullptr, const char* geometryPath = nullptr)
		{
			const char* vShaderCode = nullptr;
			std::string vertexCode;
			if (vertexPath == nullptr) vShaderCode = defaultVert;
			else
			{
				std::ifstream vShaderFile;
				vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try
				{
					vShaderFile.open(vertexPath);
					std::stringstream vShaderStream;
					vShaderStream << vShaderFile.rdbuf();
					vShaderFile.close();
					vertexCode = vShaderStream.str();

				}
				catch (std::ifstream::failure& e)
				{
					std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
				}
				vShaderCode = vertexCode.c_str();
			}

			const char* fShaderCode = nullptr;
			std::string fragmentCode;
			if (fragmentPath == nullptr) fShaderCode = defaultFrag;
			else
			{
				std::ifstream fShaderFile;
				fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try
				{
					fShaderFile.open(fragmentPath);
					std::stringstream fShaderStream;
					fShaderStream << fShaderFile.rdbuf();
					fShaderFile.close();
					fragmentCode = fShaderStream.str();
				}
				catch (std::ifstream::failure& e)
				{
					std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
				}
				fShaderCode = fragmentCode.c_str();
			}

			const char* gShaderCode = nullptr;
			std::string geometryCode;
			if (geometryPath)
			{
				std::ifstream gShaderFile;
				gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try
				{
					gShaderFile.open(geometryPath);
					std::stringstream gShaderStream;
					gShaderStream << gShaderFile.rdbuf();
					gShaderFile.close();
					geometryCode = gShaderStream.str();
				}
				catch (std::ifstream::failure& e)
				{
					std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
				}
				gShaderCode = geometryCode.c_str();
			}


			// 2. compile shaders
			unsigned int vertex, fragment, geometry;
			// vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);
			checkCompileErrors(vertex, "VERTEX");
			// fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			checkCompileErrors(fragment, "FRAGMENT");
			// geometry Shader
			if (geometryPath)
			{
				geometry = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometry, 1, &gShaderCode, NULL);
				glCompileShader(geometry);
				checkCompileErrors(geometry, "GEOMETRY");
			}
			// shader Program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			if (geometryPath) glAttachShader(ID, geometry);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			checkCompileErrors(ID, "PROGRAM");
			// delete the shaders as they're linked into our program now and no longer necessery
			glDeleteShader(vertex);
			if (geometryPath) glDeleteShader(geometry);
			glDeleteShader(fragment);

		}
		// activate the shader
		// ------------------------------------------------------------------------
		void use() const
		{
			glUseProgram(ID);
		}
		// utility uniform functions
		// ------------------------------------------------------------------------
		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		// ------------------------------------------------------------------------
		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		// ------------------------------------------------------------------------
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}
		// ------------------------------------------------------------------------
		void setVec2(const std::string& name, const glm::vec2& value) const
		{
			glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setVec2(const std::string& name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
		}
		// ------------------------------------------------------------------------
		void setVec3(const std::string& name, const glm::vec3& value) const
		{
			glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setVec3(const std::string& name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
		}
		// ------------------------------------------------------------------------
		void setVec4(const std::string& name, const glm::vec4& value) const
		{
			glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		}
		void setVec4(const std::string& name, float x, float y, float z, float w) const
		{
			glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
		}
		// ------------------------------------------------------------------------
		void setMat2(const std::string& name, const glm::mat2& mat) const
		{
			glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		void setMat3(const std::string& name, const glm::mat3& mat) const
		{
			glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		void setMat4(const std::string& name, const glm::mat4& mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

	private:
		// utility function for checking shader compilation/linking errors.
		// ------------------------------------------------------------------------
		void checkCompileErrors(GLuint shader, std::string type)
		{
			GLint success;
			GLchar infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}
	};
}


#endif