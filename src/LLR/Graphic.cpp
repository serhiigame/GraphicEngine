#include "Graphic.h"

#include "LLRVulkan.h"
#include "LLROpenGL.h"

//#include <assert.h>

namespace engine
{
	namespace graphic
	{
		Llr * Core::CreateApi(ELlrType type)
		{
			switch (type)
			{
			case ELlrType::OPEN_GL:
				return new LlrOpenGL;
			case ELlrType::VULKAN:
				return new LlrVulkan;
			default:
				//assert(false);
				break;
			}
		}
	}
}