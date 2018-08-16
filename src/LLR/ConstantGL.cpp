#include "ConstantGL.h"

namespace engine
{
	namespace graphic
	{
		ConstantGL * ConstantGL::CreateConstantGl(const size_t size)
		{
			ConstantGL * uniform = new ConstantGL;
			uniform->m_size = size;
			glGenBuffers(1, &uniform->m_uniformId);
			glBindBuffer(GL_UNIFORM_BUFFER, uniform->m_uniformId);
			glBufferData(GL_UNIFORM_BUFFER, uniform->m_size, nullptr , GL_STATIC_DRAW);

			return uniform;
		}

		ConstantGL::~ConstantGL()
		{
			glDeleteBuffers(1, &m_uniformId);
			m_uniformId = -1;
		}

		void ConstantGL::Write(const size_t offset, const size_t size, const void * data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_uniformId);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void ConstantGL::Read(const size_t offset, const size_t size, void * o_data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_uniformId);
			glGetBufferSubData(GL_UNIFORM_BUFFER, offset, size, o_data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}
}