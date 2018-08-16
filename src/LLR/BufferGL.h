#pragma once
#include "IBuffer.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{
		class IBufferGL : public IBuffer
		{
		public:
			virtual ~IBufferGL();

		public:
			static IBuffer * CreateBuffer(size_t size);

			static IBuffer * CreateIndexBuffer(size_t size);

			virtual void Init(const size_t size, const EBufferType type);

			virtual void Write(const size_t offset, const size_t size, const void * data);

			virtual void Read(const size_t offset, const size_t size, void * o_data);
			
			GLuint GetId() const {return m_bufferId; }

		private:
			GLuint m_bufferId = ((GLuint)-1);
		};
	}
}