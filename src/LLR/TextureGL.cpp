#include "TextureGL.h"

#include "LLROpenGL.h"

namespace engine
{
	
	namespace graphic
	{
		inline GLenum AdapterGlFormat(ETextureFormat format)
		{
			if (ETextureFormat::RGB == format) return GL_RGB32F;
			if (ETextureFormat::RGBA == format) return GL_RGBA32F;

			return -1;
		}


		ITexture * TextureGL::CreateTexture(const size_t width, const size_t height, const ETextureFormat format, const EDataType dataType, const ETextureType type)
		{
			TextureGL * texture = new TextureGL;

			texture->m_type = type;
			texture->m_format = format;
			texture->m_dataType = dataType;

			glGenTextures(1, &texture->m_textureId);

			if (type == ETextureType::TEXTURE_2D)
			{
				glBindTexture(GL_TEXTURE_2D, texture->m_textureId);

				glTexImage2D(GL_TEXTURE_2D, 0, AdapterGlFormat(texture->m_format), (GLsizei)width, (GLsizei)height, 0, GL_RGB, LlrOpenGL::AdapterGlDataType(texture->m_dataType), nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, texture->m_textureId);
			}

			return texture;
		}

		void TextureGL::Write(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, const void * data)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xMin, (GLint)yMin, (GLsizei)(xMax - xMin), (GLsizei)(yMax - yMin), AdapterGlFormat(m_format), LlrOpenGL::AdapterGlDataType(m_dataType), data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void TextureGL::Read(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * o_data)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			glGetTexImage(GL_TEXTURE_2D, 0, AdapterGlFormat(m_format), LlrOpenGL::AdapterGlDataType(m_dataType), o_data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}