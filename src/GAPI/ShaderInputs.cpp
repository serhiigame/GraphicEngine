#include "ShaderInputs.h"
#include "Texture2d.h"

namespace engine
{
	namespace graphic
	{
		ShaderInputConstant::~ShaderInputConstant()
		{
			if (m_constant)
			{
				delete m_constant;
			}
			m_constant = nullptr;
		}

		GeTexture2d ShaderInputTexture2d::GetTexture() const
		{
			return m_texture;
		}

		void ShaderInputTexture2d::SetTexture(const GeTexture2d & texture)
		{
			if (texture.IsValid())
			{
				m_texture = texture;
			}
		}
	}
}