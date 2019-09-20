#include "FramebufferGL.h"

#include "RenderbufferGL.h"
#include "Texture2dGL.h"

#include <vector>
#include <map>

namespace engine
{
	namespace graphic
	{
		FramebufferGL * FramebufferGL::CreateFramebuffer(const size_t width, const size_t heigth)
		{
			FramebufferGL * fb = new FramebufferGL();
			fb->m_width = width;
			fb->m_heigth = heigth;

			glGenFramebuffers(1, &fb->m_framebufferId);
			glBindFramebuffer(GL_FRAMEBUFFER, fb->m_framebufferId);

			//XXX: do nothing?
			/*
				GLuint depthrenderbuffer;
			glGenRenderbuffers(1, &depthrenderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fb->m_width, fb->m_heigth);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
			*/

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return fb;
		}

		void FramebufferGL::SetRenderbuffer(IRenderbuffer * rb)
		{
			RenderbufferGL * rbGl = ((RenderbufferGL *)rb);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, rbGl->GetId());

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbGl->GetId());

			glBindRenderbuffer(GL_RENDERBUFFER, NULL);
			glBindFramebuffer(GL_FRAMEBUFFER, NULL);
		}

		void FramebufferGL::AttachTextures2d(const Texture2dBindings & textures)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, GetId());
			
			std::vector<GLenum> drawBuffers;
			drawBuffers.resize(textures.size());

			size_t count = 0;
			for (auto textureBind : textures)
			{
				GLenum drawBuffer = GL_COLOR_ATTACHMENT0 + textureBind.first;
				const Texture2dGL * textureGL = (const Texture2dGL *)textureBind.second;
				GLuint texId = textureGL->GetId();

				glBindTexture(GL_TEXTURE_2D, texId);

				glFramebufferTexture(GL_FRAMEBUFFER, drawBuffer, texId, 0);

				drawBuffers[count] = drawBuffer;
				count++;
			}

			glDrawBuffers(drawBuffers.size(), drawBuffers.data());

			GLenum fbState = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fbState != GL_FRAMEBUFFER_COMPLETE)
				__debugbreak();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FramebufferGL::CopyTo(IFramebuffer * dst, const EFramebufferComponent components)
		{
			//XXX: NOT TESTED
			
			FramebufferGL * dstGl = ((FramebufferGL *)dst);
			
			glBindFramebuffer(GL_READ_FRAMEBUFFER, GetId());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstGl->GetId());

	
			glBlitFramebuffer(0, 0, m_width, m_heigth,
				0, 0, dstGl->m_width, dstGl->m_heigth,
				GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER, GL_NEAREST);


			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FramebufferGL::Cleare()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, GetId());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}