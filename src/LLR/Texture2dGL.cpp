#include "Texture2dGL.h"

#include "LLROpenGL.h"

namespace engine
{
	
	namespace graphic
	{
		// TODO: use in externall class
		inline GLenum AdapterGlFormat(ETextureFormat format)
		{
			if (ETextureFormat::RED == format) return GL_RED;
			if (ETextureFormat::RED_INTEGER == format) return GL_RED_INTEGER;

			if (ETextureFormat::RGB == format) return GL_RGB;
			if (ETextureFormat::RGB_INTEGER == format) return GL_RGB_INTEGER;

			if (ETextureFormat::RGBA == format) return GL_RGBA;
			if (ETextureFormat::RGBA_INTEGER == format) return GL_RGBA_INTEGER;
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


		ITexture2D * Texture2dGL::CreateTexture(const size_t width, const size_t height, const ETextureFormat usageFormat, const ETextureSizedFormat internalFormat, const EDataType dataType)
		{
			Texture2dGL * texture = new Texture2dGL;


			texture->m_dataType = dataType;
			texture->m_format = usageFormat;
			texture->m_sizedFormat = internalFormat;

			glGenTextures(1, &texture->m_textureId);

			glBindTexture(GL_TEXTURE_2D, texture->m_textureId);


			glTexImage2D(GL_TEXTURE_2D, 0, AdapterGlSizedFormat(internalFormat), (GLsizei)width, (GLsizei)height, 0, AdapterGlFormat(usageFormat), LlrOpenGL::AdapterGlDataType(texture->m_dataType), nullptr);

			
			

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
			glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xMin, (GLint)yMin, (GLsizei)(xMax - xMin), (GLsizei)(yMax - yMin), AdapterGlFormat(m_format), LlrOpenGL::AdapterGlDataType(m_dataType), data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture2dGL::Read(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * o_data)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			glGetTexImage(GL_TEXTURE_2D, 0, AdapterGlFormat(m_format), LlrOpenGL::AdapterGlDataType(m_dataType), o_data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}