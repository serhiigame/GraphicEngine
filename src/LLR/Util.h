#pragma once

#include "LLREnum.h"

#include <GLEW/glew.h>


namespace engine
{
	namespace graphic
	{

		class Util
		{
		public:
			static GLenum TextureFormat2GLenum(ETextureFormat format);

			static GLenum TextureSizedFormat2GLenum(ETextureSizedFormat format);

			static GLenum DataType2GLenum(const EDataType dataType);

			static GLenum CobeMapFace2GLenum(ETextureCubeMapFace face);

		};
	}
}
