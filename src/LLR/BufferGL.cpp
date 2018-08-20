#include "BufferGL.h"

namespace engine
{
	namespace graphic
	{

		IBuffer * IBufferGL::CreateBuffer(size_t size)
		{
			IBufferGL * buffer = new IBufferGL;
			buffer->Init(size, EBufferType::VERTEX);

			glGenBuffers(1, &buffer->m_bufferId);

			glBindBuffer(GL_ARRAY_BUFFER, buffer->m_bufferId);
			glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

			return buffer;
		}

		IBuffer * IBufferGL::CreateIndexBuffer(size_t size)
		{
			IBufferGL * buffer = new IBufferGL;

			buffer->Init(size, EBufferType::INDEX);

			glGenBuffers(1, &buffer->m_bufferId);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->m_bufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

			return buffer;
		}

		IBufferGL::~IBufferGL()
		{
			glDeleteBuffers(1, &m_bufferId);
			m_bufferId = -1;
		}

		void IBufferGL::Init(const size_t size, const EBufferType type)
		{
			m_Size = size;
			m_Type = type;
		}

		void IBufferGL::Write(const size_t offset, const size_t size, const void * data)
		{
			if (m_Type == EBufferType::VERTEX)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
				glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else if (m_Type == EBufferType::INDEX)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}

		void IBufferGL::Read(const size_t offset, const size_t size, void * o_data)
		{
			if (m_Type == EBufferType::VERTEX)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
				glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, o_data);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else if (m_Type == EBufferType::INDEX)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
				glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, o_data);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
	}
}