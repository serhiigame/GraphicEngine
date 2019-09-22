#include "TextureCubeMapGL.h"

#include "LLROpenGL.h"

#include "Util.h"

namespace engine
{
	
	namespace graphic
	{

		ITextureCubeMap * TextureCubeMapGL::CreateTexture(const size_t width, const size_t height, const ETextureFormat usageFormat, const ETextureSizedFormat internalFormat, const EDataType dataType)
		{
			TextureCubeMapGL * texture = new TextureCubeMapGL;

			
			texture->m_dataType = dataType;
			texture->m_format = usageFormat;

			glGenTextures(1, &texture->m_textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_textureId);


			for (GLuint i = 0; i < 6; i++)
			{
				//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Util::TextureSizedFormat2GLenum(internalFormat), (GLsizei)width, (GLsizei)height, 0, Util::TextureFormat2GLenum(usageFormat), Util::DataType2GLenum(texture->m_dataType), nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Util::TextureSizedFormat2GLenum(internalFormat), (GLsizei)width, (GLsizei)height, 0, Util::TextureFormat2GLenum(usageFormat), Util::DataType2GLenum(texture->m_dataType), nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			return texture;
		}

		void TextureCubeMapGL::Write(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, const void * data)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
			glTexSubImage2D(Util::CobeMapFace2GLenum(face), 0, (GLint)xMin, (GLint)yMin, (GLsizei)(xMax - xMin), (GLsizei)(yMax - yMin), Util::TextureFormat2GLenum(m_format), Util::DataType2GLenum(m_dataType), data);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		void TextureCubeMapGL::Read(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * o_data)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
			glGetTexImage(Util::CobeMapFace2GLenum(face), 0, Util::TextureFormat2GLenum(m_format), Util::DataType2GLenum(m_dataType), o_data);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

	}
}