#pragma once

#include "Math.h"

#include <vector>

namespace engine
{
	namespace graphic
	{
		struct RawMeshData 
		{
			std::string Name;
			std::vector<Vec3f> Positions;
			std::vector<Vec2f> Uv;
			std::vector<Vec3f> Normals;
			std::vector<unsigned int> Indexes;
		};
	}
}