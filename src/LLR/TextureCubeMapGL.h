#pragma once
#include "ITextureCubeMap.h"



namespace engine
{
	namespace graphic
	{
		class TextureCubeMapGL : public ITextureCubeMap
		{
		public:
			static ITextureCubeMap * CreateTexture(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType);

			virtual void Write(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, const void * data) override;

			virtual void Read(ETextureCubeMapFace face, const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * o_data) override;

			GLuint GetId() const { return m_textureId; }
		private:
			GLuint m_textureId = (GLuint) -1;
		};
	}
}