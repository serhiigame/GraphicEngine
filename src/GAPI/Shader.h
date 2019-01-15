#pragma once

#include "IResource.h"

namespace engine
{
	namespace graphic
	{
		class IShader;
		
		class IShaderInput;

		class Shader : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:
			~Shader();

			void SetShaderInput(IShaderInput * shaderInput, const int location);

			bool IsValid() { return m_shader != nullptr; }

		protected:
			IShader * m_shader = nullptr;
			int m_priority = -1;
		};
	}
}