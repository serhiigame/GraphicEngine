#include "Texture2d.h"

namespace engine
{
	namespace graphic
	{
		Texture2d::~Texture2d()
		{
			if (m_texture)
			{
				delete m_texture;
			}
			m_texture = nullptr;
		}
	}
}