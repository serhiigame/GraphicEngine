#pragma once

#include "Material.h"

namespace engine
{
	namespace graphic
	{
		enum class EMaterialType
		{
			NONE = -1
			, PHONG = 0
		};

		class MaterialFactory
		{
		public:
			static Material CreateMaterial(EMaterialType materialType);
		};
	}
}