#pragma once

#include <Mathematics/GteVector3.h>
#include <vector>

namespace engine
{
	namespace graphic
	{
		typedef gte::Vector3<float> Vec3f;
		struct RawMeshData 
		{
			std::vector<Vec3f> Positions;
			std::vector<Vec3f> Normals;
			std::vector<unsigned int> Indexes;
		};
	}
}