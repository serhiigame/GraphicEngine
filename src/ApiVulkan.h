#pragma once
#include "IApi.h"

namespace engine
{
	namespace graphic
	{
		class VulcanImpl;
		class ApiVulkan final : public IApi
		{
		public:
			ApiVulkan();
			~ApiVulkan();

			virtual void Init(IWindow * window);

			virtual IShader * CreateShader(const std::string & vertexPass
				, const std::string & fragmentPath
				, const std::string & tesselationPath = std::string()
				, const std::vector<std::string> define = std::vector<std::string>());

			virtual IBuffer * CreateBuffer(size_t size, EBufferType type);

			virtual IConstant * CreateConatant(const size_t size);

			virtual ITexture * CreateTexture(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType, const ETextureType type);

			virtual IFramebuffer * CreateFramebuffer(const size_t width, const size_t heigth);

			virtual IRenderPass * CreateRenderPass() { return nullptr; }

		private:
			VulcanImpl * m_impl = nullptr;
			IWindow * m_window = nullptr;

		};
	}
}
