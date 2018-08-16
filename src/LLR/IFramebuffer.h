#pragma once

#include "IShader.h"

namespace engine
{
	namespace graphic
	{		
		class IFramebuffer
		{
		public:
			virtual void AttachTextures(const TextureBindings & textures) = 0;
			virtual void Cleare() = 0;
		};
	}
}