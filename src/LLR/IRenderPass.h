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
			
			virtual void EnableStencilWriting() = 0;

			virtual void DisableStencilWriting() = 0;
			
			virtual void SetStencilParams(EStencilFunc func, const int ref, EStencilOp passOp, EStencilOp failOp) = 0;
		};
	}
}