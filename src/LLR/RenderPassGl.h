#pragma once

#include "IRenderPass.h"


namespace engine
{
	namespace graphic
	{
		
		class RenderPassGL : public IRenderPass
		{
		public:
			static IRenderPass * CreateRenderPass();

			virtual EApiRenderPassStatus Execute(IShader * shader, IFramebuffer * outputBuffer = nullptr) override;
		};
	}
}
