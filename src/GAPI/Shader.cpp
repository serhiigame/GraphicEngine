#include "Shader.h"
#include "ShaderInputs.h"

#include "..\LLR\IShader.h"
#include "..\LLR\IConstant.h"

namespace engine
{
	namespace graphic
	{
		Shader::~Shader()
		{
			if (m_shader)
			{
				delete m_shader;
			}
			m_shader = nullptr;
		}

		void Shader::SetShaderInput(IShaderInput * shaderInput, const int location)
		{
			switch (shaderInput->GetShaderInputType())
			{
			case EShaderInputType::CONSTANT:
			{
				const IConstant * constant = ((ShaderInputConstant *)shaderInput)->m_constant;
				m_shader->AttachConstant(constant, location);
				break;
			}
			default:
				break;
			}
		}
	}
}