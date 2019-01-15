#pragma once
#include "IRenderbuffer.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{	
		class RenderbufferGL : public IRenderbuffer
		{
		public:
			static RenderbufferGL * CreateRenderbuffer(const size_t width, const size_t heigth);

			static void DeleteRenderbuffer(RenderbufferGL * renderbuffer);
			
			GLuint GetId() const { return m_renderbufferId; }

		private:
			GLuint m_renderbufferId = ((GLuint)-1);

			size_t	m_width = 0;
			size_t	m_heigth = 0;
		};
	}
}