#include "Util.h"

#include "LLROpenGL.h"


namespace engine
{
	namespace graphic
	{

			// TODO: use in externall class
			GLenum Util::TextureFormat2GLenum(ETextureFormat format)
			{
				if (ETextureFormat::RED == format) return GL_RED;
				if (ETextureFormat::RED_INTEGER == format) return GL_RED_INTEGER;

				if (ETextureFormat::RGB == format) return GL_RGB;
				if (ETextureFormat::RGB_INTEGER == format) return GL_RGB_INTEGER;

				if (ETextureFormat::RGBA == format) return GL_RGBA;
				if (ETextureFormat::RGBA_INTEGER == format) return GL_RGBA_INTEGER;
				return -1;
			}


			GLenum Util::TextureSizedFormat2GLenum(ETextureSizedFormat format)
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

			GLenum Util::DataType2GLenum(const EDataType dataType)
			{
				switch (dataType)
				{
				case EDataType::FLOAT:			return GL_FLOAT;
				case EDataType::UNSIGNED_INT:	return GL_UNSIGNED_INT;
				default:						break;
				}

				return NULL;
			};


			GLenum Util::CobeMapFace2GLenum(ETextureCubeMapFace face)
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
	}
}