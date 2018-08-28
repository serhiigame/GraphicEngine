#pragma once
#include "IShader.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace engine
{
	namespace graphic
	{
		class ShaderVulkan : public IShader
		{
			friend class LlrVulkan;

			virtual void AttachAttribute(const IBuffer * buffer, const int location, const size_t offset, const size_t count, const EDataType & type) {}

			virtual void AttachConstant(const IConstant * constant, const int location)  override {};

			virtual void AttachTexture2d(const ITexture2D * constant, const int location) override {};

			virtual void AttachTextureCubeMap(const ITextureCubeMap * texture, const int location) {};
		private:

			VkShaderModule m_vertexShader;
			VkShaderModule m_fragmentShader;

		};
	}
}