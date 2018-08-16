#include "Llr.h"

#include "LLROpenGL.h"

namespace engine
{
	namespace graphic
	{
		Llr * Llr::CreateLlr(ELlrType type)
		{
			switch (type)
			{
			case ELlrType::OPEN_GL:
				return new LlrOpenGL;
			//case ELlrType::VULKAN:
			//	return new LlrVulkan;
			default:
				return nullptr;
			}
		}

		void Llr::DeleteLlr(Llr *& llr)
		{
			delete llr;
			llr = nullptr;
		}
	}
}