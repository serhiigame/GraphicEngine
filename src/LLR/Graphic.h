#pragma once
#include "Llr.h"

namespace engine
{
	namespace graphic
	{
		class Core
		{
		public:
			Llr * CreateApi(ELlrType type);
		};
	}
}