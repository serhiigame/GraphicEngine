#pragma once
#include "Llr.h"

namespace engine
{
	namespace graphic
	{
		
		class LlrOpenGL : public Llr
		{
		public:
			static GLenum AdapterGlDataType(const EDataType dataType);
			
			virtual IShader * CreateShader(const std::string & vertexPass
				, const std::string & fragmentPass
				, const std::string & tesselationPass = std::string()
				, const std::vector<std::string> define = std::vector<std::string>()) override;

			virtual IBuffer * CreateBuffer(size_t size) override;

			virtual IBuffer * CreateIndexBuffer(size_t size) override;

			virtual IConstant * CreateConatant(const size_t size) override;

			virtual ITexture * CreateTexture(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType, const ETextureType type) override;

			virtual IFramebuffer * CreateFramebuffer(const size_t width, const size_t heigth) override;

			virtual IRenderPass * CreateRenderPass() override;
		};
	}
}
