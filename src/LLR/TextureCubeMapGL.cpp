#include "TextureCubeMapGL.h"

#include "LLROpenGL.h"

namespace engine
{
	
	namespace graphic
	{
		// TODO: use in externall class
		inline GLenum AdapterGlFormat(ETextureFormat format)
		{
			if (ETextureFormat::RED == format) return GL_RED;
			if (ETextureFormat::RGB == format) return GL_RGB;
			if (ETextureFormat::RGBA == format) return GL_RGBA;
			return -1;
		}


		inline GLenum AdapterGlSizedFormat(ETextureSizedFormat format)
		{
			switch (format)
			{
			case ETextureSizedFormat::RED_UNSIGNED_INT_32:
				return GL_R32UI;
			case ETextureSizedFormat::RED_FLOAT:
				return GL_R32F;
			case ETextureSizedFormat::RGB_UNSIGNED_INT_32:
				return GL_RGB32UI;
			case ETextureSizedFormat::RGB_FLOAT:
				return GL_RGB32F;
			case ETextureSizedFormat::RGBA_UNSIGNED_INT_32:
				return GL_RGBA32UI;
			case ETextureSizedFormat::RGBA_FLOAT:
				return GL_RGBA32F;
			case ETextureSizedFormat::NONE:
			default:
				break;
			}
			return -1;
		}

		inline GLenum AdapterGlCobeMapFace(ETextureCubeMapFace face)
		{
			switch (face)
			{
			case engine::graphic::ETextureCubeMapFace::BACK:
				return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

			case engine::graphic::ETextureCubeMapFace::FRONT:
				return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;

			case engine::graphic::ETextureCubeMapFace::LEFT:
				return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;

			case engine::graphic::ETextureCubeMapFace::RIGHT:
				return GL_TEXTURE_CUBE_MAP_POSITIVE_X;

			case engine::graphic::ETextureCubeMapFace::TOP:
				return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;

			case engine::graphic::ETextureCubeMapFace::BUTTOM:
				return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;

			default:
				break;
			}
				
			return NULL;
		}


		ITextureCubeMap * TextureCubeMapGL::CreateTexture(const size_t width, const size_t height, const ETextureFormat usageFormat, const ETextureSizedFormat internalFormat, const EDataType dataType)
		{
			TextureCubeMapGL * texture = new TextureCubeMapGL;

			
			texture->m_dataType = dataType;

			glGenTextures(1, &texture->m_textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_textureId);


			for (GLuint i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, AdapterGlFormat(usageFormat), (GLsizei)width, (GLsizei)height, 0, AdapterGlSizedFormat(internalFormat), LlrOpenGL::AdapterGlDataType(texture->m_dataType), nullptr);
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
			glTexSubImage2D(AdapterGlCobeMapFace(face), 0, (GLint)xMin, (GLint)yMin, (GLsizei)(xMax - xMin), (GLsizei)(yMax - yMin), AdapterGlFormat(m_format), LlrOpenGL::AdapterGlDataType(m_dataType), data);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		void TextureCubeMapGL::Read(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * o_data)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
			glGetTexImage(AdapterGlCobeMapFace(face), 0, AdapterGlFormat(m_format), LlrOpenGL::AdapterGlDataType(m_dataType), o_data);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

	}
}