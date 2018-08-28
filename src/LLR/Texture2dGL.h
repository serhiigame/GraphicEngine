#pragma once
#include "ITexture2D.h"



namespace engine
{
	namespace graphic
	{
		class Texture2dGL : public ITexture2D
		{
		public:
			static ITexture2D * CreateTexture(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType);

			virtual void Write(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, const void * o_data) override;

			virtual void Read(const size_t xMin, const size_t xMax, size_t yMin, size_t yMax, void * data) override;

			GLuint GetId() const { return m_textureId; }
		private:
			GLuint m_textureId = (GLuint) -1;
		};
	}
}