#include "Texture2d.h"

#include "../LLR/ITexture2D.h"

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

		void Texture2d::WriteImage(int xOffset, int yOffset, int w, int h, const void * data)
		{
			m_texture->Write(xOffset, w, yOffset, h, data);
		}
	}
}