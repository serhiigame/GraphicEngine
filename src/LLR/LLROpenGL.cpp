#include "LLROpenGL.h"
#include "ShaderOpenGL.h"
#include "BufferGL.h"
#include "ConstantGL.h"
#include "Texture2dGL.h"
#include "TextureCubeMapGL.h"
#include "FramebufferGL.h"
#include "RenderbufferGL.h"
#include "RenderPassGl.h"

#include <GLEW/glew.h>

#include <assert.h>
#include <fstream>


namespace engine
{
	namespace graphic
	{
		GLenum LlrOpenGL::AdapterGlDataType(const EDataType dataType)
		{
			switch (dataType)
			{
			case EDataType::FLOAT:			return GL_FLOAT;
			case EDataType::UNSIGNED_INT:	return GL_UNSIGNED_INT;
			default:						return NULL;
			}
		};

		IShader * LlrOpenGL::CreateShader(const std::string & vertexPass, const std::string & fragmentPass, const std::string & tesselationPass, const std::vector<std::string> define)
		{
			GLint statusV, statusF;
			GLuint vertexShaderId, fragmentShaderId;
			std::vector<char> vertexShader, fragmentShader, shaderInfo;

			{
				std::ifstream file(vertexPass.c_str(), std::ios::ate);

				if (!file.is_open()) {
					// Log error
					return nullptr;
				}

				size_t fileSize = (size_t)file.tellg();
				vertexShader.resize(fileSize);

				file.seekg(0);
				file.read(vertexShader.data(), fileSize);
			}

			{
				std::ifstream file(fragmentPass.c_str(), std::ios::ate);

				if (!file.is_open()) {
					// Log error
					return nullptr;
				}

				size_t fileSize = (size_t)file.tellg();
				fragmentShader.resize(fileSize);

				file.seekg(0);
				file.read(fragmentShader.data(), fileSize);
			}

			return ShaderOpenGL::CreateShader(vertexShader.data(), fragmentShader.data(), nullptr);
			/*vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
			fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);


			char * strPtr = vertexShader.data();
			glShaderSource(vertexShaderId, 1, (const GLchar **)&strPtr, NULL);

			strPtr = fragmentShader.data();
			glShaderSource(fragmentShaderId, 1, (const GLchar **)&strPtr, NULL);

			glCompileShader(vertexShaderId);
			glCompileShader(fragmentShaderId);

			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &statusV);
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &statusF);


			if (!statusV)
			{
				GLint maxLength = 0;
				glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

				GLchar * msg = new GLchar[maxLength];

				glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, msg);
				glDeleteShader(vertexShaderId);

				printf(msg);

				assert(false);
			}

			if (!statusF)
			{
				GLint maxLength = 0;
				glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);

				GLchar * msg = new GLchar[maxLength];

				glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, msg);
				glDeleteShader(vertexShaderId);

				printf(msg);

				assert(false);
			}

			ShaderOpenGL * shader = new ShaderOpenGL;
			shader->m_programId = glCreateProgram();


			glAttachShader(shader->m_programId, vertexShaderId);
			glAttachShader(shader->m_programId, fragmentShaderId);

			glLinkProgram(shader->m_programId);

			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);


			glUseProgram(shader->m_programId);
			glGenVertexArrays(1, &shader->m_vao);
			glUseProgram(0);

			return shader;*/
		}

		IBuffer * LlrOpenGL::CreateBuffer(size_t size)
		{
			return IBufferGL::CreateBuffer(size);
		}

		IBuffer * LlrOpenGL::CreateIndexBuffer(size_t size)
		{
			return IBufferGL::CreateIndexBuffer(size);
		}

		IConstant * LlrOpenGL::CreateConatant(const size_t size)
		{
			return ConstantGL::CreateConstantGl(size);
		}

		ITexture2D * LlrOpenGL::CreateTexture2d(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType)
		{
			return Texture2dGL::CreateTexture(width, heigth, format, dataType);
		}

		ITextureCubeMap * LlrOpenGL::CreateTextureCubeMap(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType)
		{
			return TextureCubeMapGL::CreateTexture(width, heigth, format, dataType);
		}

		IFramebuffer * LlrOpenGL::CreateFramebuffer(const size_t width, const size_t heigth)
		{
			return FramebufferGL::CreateFramebuffer(width, heigth);
		}

		IRenderbuffer * LlrOpenGL::CreateRenderbuffer(const size_t width, const size_t heigth)
		{
			return RenderbufferGL::CreateRenderbuffer(width, heigth);
		}

		IRenderPass * LlrOpenGL::CreateRenderPass()
		{
			return RenderPassGL::CreateRenderPass();
		}
	}
}