#pragma once

#include "LLREnum.h"

#include "IShader.h"

#include "IRenderbuffer.h"

namespace engine
{
	namespace graphic
	{		
		class IFramebuffer
		{
		public:
			virtual void SetRenderbuffer(IRenderbuffer * rb) = 0;
			
			virtual void AttachTextures2d(const Texture2dBindings & textures) = 0;

			virtual void CopyTo(IFramebuffer * dstVb, const EFramebufferComponent components) = 0;

			virtual void Cleare() = 0;
		};
	}
}