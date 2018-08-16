#include "Texture.h"

namespace engine
{
	namespace graphic
	{
		Texture::~Texture()
		{
			if (m_texture)
			{
				delete m_texture;
			}
			m_texture = nullptr;
		}
	}
}