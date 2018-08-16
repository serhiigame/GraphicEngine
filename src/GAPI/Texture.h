#pragma once

#include "IResource.h"

namespace engine
{
	namespace graphic
	{
		class ITexture;
		
		class Texture : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:
			~Texture();

		protected:
			ITexture * m_texture = nullptr;
		};
	}
}