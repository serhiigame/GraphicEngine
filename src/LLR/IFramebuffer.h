#pragma once

#include "IShader.h"

namespace engine
{
	namespace graphic
	{		
		class IFramebuffer
		{
		public:
			virtual void AttachTextures2d(const Texture2dBindings & textures) = 0;
			virtual void Cleare() = 0;
		};
	}
}