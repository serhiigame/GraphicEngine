#pragma once

#include "api.h"

#include "IResource.h"

namespace engine
{
	namespace graphic
	{
		class ITexture2D;
		
		class Texture2d : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:
			~Texture2d();

			GAPI_EXPORT
			void WriteImage(int xOffset, int yOffset, int w, int h, const void * data);

		protected:
			ITexture2D * m_texture = nullptr;
		};
	}
}