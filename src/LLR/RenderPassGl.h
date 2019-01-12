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

			virtual void EnableStencilWriting() override;

			virtual void DisableStencilWriting() override;

			virtual void SetStencilParams(EStencilFunc func, const int ref, EStencilOp passOp, EStencilOp failOp) override;
		};
	}
}
