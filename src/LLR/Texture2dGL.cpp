#include "Texture2dGL.h"

#include "LLROpenGL.h"

#include "Util.h"

namespace engine
{
	
	namespace graphic
	{
		ITexture2D * Texture2dGL::CreateTexture(const size_t width, const size_t height, const ETextureFormat usageFormat, const ETextureSizedFormat internalFormat, const EDataType dataType)
		{
			Texture2dGL * texture = new Texture2dGL;


			texture->m_dataType = dataType;
			texture->m_format = usageFormat;
			texture->m_sizedFormat = internalFormat;

			glGenTextures(1, &texture->m_textureId);

			glBindTexture(GL_TEXTURE_2D, texture->m_textureId);


			glTexImage2D(GL_TEXTURE_2D, 0, Util::TextureSizedFormat2GLenum(internalFormat), (GLsizei)width, (GLsizei)height, 0, Util::TextureFormat2GLenum(usageFormat), Util::DataType2GLenum(texture->m_dataType), nullptr);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, texture->m_textureId);

			return texture;
		}

		void Texture2dGL::Write(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, const void * data)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xMin, (GLint)yMin, (GLsizei)(xMax - xMin), (GLsizei)(yMax - yMin), Util::TextureFormat2GLenum(m_format), Util::DataType2GLenum(m_dataType), data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture2dGL::Read(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * o_data)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			glGetTexImage(GL_TEXTURE_2D, 0, Util::TextureFormat2GLenum(m_format), Util::DataType2GLenum(m_dataType), o_data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}