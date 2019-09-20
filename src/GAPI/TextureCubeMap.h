#pragma once

#include "api.h"

#include "IResource.h"
#include "../LLR/LLREnum.h"

namespace engine
{
	namespace graphic
	{
		class ITextureCubeMap;
		
		class TextureCubeMap : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:
			~TextureCubeMap();

			GAPI_EXPORT
			void WriteFace(ETextureCubeMapFace, const float * data);

		protected:
			ITextureCubeMap * m_texture = nullptr;
			size_t m_size;
		};
	}
}