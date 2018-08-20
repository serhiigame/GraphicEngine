#include "FramebufferGL.h"

#include "TextureGL.h"

#include <vector>
#include <map>

namespace engine
{
	namespace graphic
	{
		FramebufferGL * FramebufferGL::CreateFramebuffer(const size_t width, const size_t heigth)
		{
			FramebufferGL * fb = new FramebufferGL();

			glGenFramebuffers(1, &fb->m_framebufferId);
			glBindFramebuffer(GL_FRAMEBUFFER, fb->m_framebufferId);

			GLuint depthrenderbuffer;
			glGenRenderbuffers(1, &depthrenderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, heigth);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return fb;
		}

		void FramebufferGL::AttachTextures(const TextureBindings & textures)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, GetId());
			
			std::vector<GLenum> drawBuffers;
			drawBuffers.resize(textures.size());

			size_t count = 0;
			for (auto textureBind : textures)
			{
				GLenum drawBuffer = GL_COLOR_ATTACHMENT0 + textureBind.first;
				const TextureGL * textureGL = (const TextureGL *)textureBind.second;
				GLuint texId = textureGL->GetId();

				glBindTexture(GL_TEXTURE_2D, texId);

				glFramebufferTexture(GL_FRAMEBUFFER, drawBuffer, texId, 0);

				drawBuffers[count] = drawBuffer;
				count++;
			}

			glDrawBuffers(drawBuffers.size(), drawBuffers.data());

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				__debugbreak();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FramebufferGL::Cleare()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, GetId());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}