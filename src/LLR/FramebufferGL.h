#pragma once
#include "IFramebuffer.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{	
		class IRenderbuffer;
		
		class FramebufferGL : public IFramebuffer
		{
		public:
			static FramebufferGL * CreateFramebuffer(const size_t width, const size_t heigth);

			virtual void SetRenderbuffer(IRenderbuffer * rb) override;

			virtual void AttachTextures2d(const Texture2dBindings & textures) override;

			virtual void CopyTo(IFramebuffer * dstVb, const EFramebufferComponent components) override;
			
			virtual void Cleare() override;

			GLuint GetId() const { return m_framebufferId; }

		private:
			GLuint m_framebufferId;

			size_t	m_width = 0;
			size_t	m_heigth = 0;
		};
	}
}