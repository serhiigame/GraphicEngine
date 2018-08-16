#pragma once

#include "LLREnum.h"

namespace engine
{
	namespace graphic
	{
		class IShader;

		class IFramebuffer;

		class IRenderPass
		{
		public:

			virtual EApiRenderPassStatus Execute(IShader * shader, IFramebuffer * outputBuffer = nullptr) = 0;

		};
	}
}