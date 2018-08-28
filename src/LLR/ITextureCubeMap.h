#pragma once

#include "LLREnum.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{
		class ITextureCubeMap
		{
		public:
			virtual void Write(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, const void * o_data) = 0;

			virtual void Read(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * data) = 0;

		protected:
			ETextureFormat m_format = ETextureFormat::NONE;
			EDataType m_dataType = EDataType::NONE;
		};
	}
}