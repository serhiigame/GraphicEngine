#pragma once
#include "IFramebuffer.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{	
		class FramebufferGL : public IFramebuffer
		{
		public:
			static FramebufferGL * CreateFramebuffer(const size_t width, const size_t heigth);

			virtual void AttachTextures(const TextureBindings & textures) override;

			virtual void Cleare() override;

			GLuint GetId() const { return m_framebufferId; }

		private:
			GLuint m_framebufferId;
		};
	}
}