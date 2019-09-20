#include "TextureCubeMap.h"

#include "../LLR/TextureCubeMapGL.h"

namespace engine
{
	namespace graphic
	{
		TextureCubeMap::~TextureCubeMap()
		{
			if (m_texture)
			{
				delete m_texture;
			}

			m_texture = nullptr;
		}
		void TextureCubeMap::WriteFace(ETextureCubeMapFace face, const float * data)
		{
			if (m_texture)
			{
				m_texture->Write(face, 0, m_size, 0, m_size, data);
			}
		}
	}
}