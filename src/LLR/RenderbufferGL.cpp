#include "RenderbufferGL.h"

#include "Texture2dGL.h"

#include <vector>
#include <map>

namespace engine
{
	namespace graphic
	{
		RenderbufferGL * RenderbufferGL::CreateRenderbuffer(const size_t width, const size_t heigth)
		{
			RenderbufferGL * rb = new RenderbufferGL();
			rb->m_width = width;
			rb->m_heigth = heigth;

			glGenRenderbuffers(1, &rb->m_renderbufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, rb->m_renderbufferId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, rb->m_width, rb->m_heigth);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb->m_renderbufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, NULL);

			return rb;
		}
		void RenderbufferGL::DeleteRenderbuffer(RenderbufferGL * rb)
		{
			glDeleteRenderbuffers(1, &rb->m_renderbufferId);
			rb->m_renderbufferId = ((GLuint)-1);
			rb->m_width = 0;
			rb->m_heigth = 0;
		}
	}
}