#pragma once

#include "ShaderInputs.h"

namespace engine
{
	namespace graphic
	{
		class IShader;
		
		class IShaderInput;
		class ShaderInputInfoBase;

		struct ShaderInfo
		{
			std::string vertShaderPath;
			std::string fragShaderPath;

			std::vector<ShaderInputInfoBase *> Inputs;
			std::vector<ShaderOutputInfo> Outputs;
		};

		class Shader
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