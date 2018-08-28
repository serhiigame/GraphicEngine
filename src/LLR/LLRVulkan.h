#pragma once
#include "Llr.h"

namespace engine
{
	namespace graphic
	{
		class VulcanImpl;
		class LlrVulkan final : public Llr
		{
		public:
			LlrVulkan();
			~LlrVulkan();

			virtual void Init(IWindow * window);

			virtual IShader * CreateShader(const std::string & vertexPass
				, const std::string & fragmentPath
				, const std::string & tesselationPath = std::string()
				, const std::vector<std::string> define = std::vector<std::string>());


			virtual IBuffer * CreateBuffer(size_t size);

			virtual IBuffer * CreateIndexBuffer(size_t size) { return nullptr; }

			virtual IConstant * CreateConatant(const size_t size);

			virtual ITexture2D * CreateTexture2d(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType);

			virtual ITextureCubeMap * CreateTextureCubeMap(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType);

			virtual IFramebuffer * CreateFramebuffer(const size_t width, const size_t heigth);

			virtual IRenderPass * CreateRenderPass() { return nullptr; }

			//virtual EApiRenderPassStatus RenderPass(const IShader * shader, IFramebuffer * outputBuffer = nullptr);

		private:
			VulcanImpl * m_impl = nullptr;
			IWindow * m_window = nullptr;

		};
	}
}
