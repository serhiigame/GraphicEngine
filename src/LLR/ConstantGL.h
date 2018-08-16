#pragma once

#include "IConstant.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{
		class ConstantGL : public IConstant
		{
		public:
			~ConstantGL();
		public:
			static ConstantGL * CreateConstantGl(const size_t size);

			virtual void Write(const size_t offset, const size_t size, const void * data);

			virtual void Read(const size_t offset, const size_t size, void * o_data);

			GLuint GetId() const { return m_uniformId; }
		private:
			GLuint m_uniformId = ((GLuint)-1);
		};
	}
}