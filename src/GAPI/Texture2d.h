#pragma once

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

		protected:
			ITexture2D * m_texture = nullptr;
		};
	}
}