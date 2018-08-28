#include "ShaderOpenGL.h"

#include "LlrOpenGL.h"
#include "BufferGL.h"
#include "ConstantGL.h"
#include "Texture2dGL.h"
#include "TextureCubeMapGL.h"

#include <GLEW/glew.h>

#include <assert.h>

namespace engine
{
	namespace graphic
	{
		ShaderOpenGL * ShaderOpenGL::CreateShader(const char * vertCodeSrc, const char * fragCodeSrc, const char * tessCodeSrc)
		{
			GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);


			glShaderSource(vertexShaderId, 1, (const GLchar **)&vertCodeSrc, NULL);
			glShaderSource(fragmentShaderId, 1, (const GLchar **)&fragCodeSrc, NULL);

			glCompileShader(vertexShaderId);
			glCompileShader(fragmentShaderId);

			GLint statusV, statusF;
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
			//glGenVertexArrays(1, &shader->m_vao); // !!!
			glUseProgram(0);

			return shader;
		}

		ShaderOpenGL::~ShaderOpenGL()
		{
			glDeleteProgram(m_programId);
			m_programId = ((GLuint)-1);
		}

		void ShaderOpenGL::AttachAttribute(const IBuffer * buffer, const int location, const size_t stride, const size_t count, const EDataType & dataType)
		{
			glUseProgram(m_programId);
			
			EBufferType bufferType = buffer->GetBufferType();
			GLenum type = LlrOpenGL::AdapterGlDataType(dataType);
			if (bufferType == EBufferType::VERTEX)
			{
				const IBufferGL * buffGl = (const IBufferGL *)buffer;

				//glBindVertexArray(m_vao);
				glBindBuffer(GL_ARRAY_BUFFER, buffGl->GetId());

				glVertexAttribPointer(location, (GLsizei)count, type, GL_FALSE, (GLsizei)stride, nullptr);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindVertexArray(0); 

				m_vertexAtributeIds.insert(location);
			}
			if (buffer->GetBufferType() == EBufferType::INDEX)
			{
				const IBufferGL * bufferGl = (const IBufferGL *)buffer;
				m_indexIds.emplace(bufferGl->GetId(), count);
			}

			glUseProgram(0);
		}

		void ShaderOpenGL::AttachConstant(const IConstant * constant, const int location)
		{
			const ConstantGL * constantGl = (const ConstantGL *)constant;
			
			glBindBufferBase(GL_UNIFORM_BUFFER, location, constantGl->GetId());
			
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glUseProgram(0);

			m_uniformBlock.emplace(location, constantGl->GetId());
		}
		void ShaderOpenGL::AttachTexture2d(const ITexture2D * texture, const int location)
		{
			const Texture2dGL * texGl = (const Texture2dGL *) texture;
			const GLuint texId = texGl->GetId();

			m_texture2dAttachment.emplace(location, texId);
		}
		void ShaderOpenGL::AttachTextureCubeMap(const ITextureCubeMap * texture, const int location)
		{
			const TextureCubeMapGL * texGl = (const TextureCubeMapGL *)texture;
			const GLuint texId = texGl->GetId();

			m_textureCubeMapAttachment.emplace(location, texId);
		}
	}
}