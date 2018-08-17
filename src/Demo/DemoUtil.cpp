#include "DemoUtil.h"

#define _USE_MATH_DEFINES
#include <math.h>


static void computeSphereVertex(float radius, float theta, float phi, engine::graphic::Vec3f & out_position, engine::graphic::Vec3f & out_normal)
{
	out_position = engine::graphic::Vec3f({ radius * sin(theta) * cos(phi) , radius * sin(theta) * sin(phi) , radius * cos(theta) });

	float len = sqrtf(out_position[0] * out_position[0] + out_position[1] * out_position[1] + out_position[2] * out_position[2]);
	out_normal = engine::graphic::Vec3f({ out_position[0] / len, out_position[1] / len, out_position[2] / len });
}


engine::graphic::RawMeshData DemoUtils::CreatePlane(const float width, const float height)
{
	engine::graphic::RawMeshData meshData;

	/*meshData.Positions = {
		{ - width *0.5f ,  0.0f, -height *0.5f }
		,{ -width *0.5f,  0.0f, height *0.5f }
		,{ width *0.5f ,  0.0f, height *0.5f}
		,{ width *0.5f ,  0.0f, -height *0.5f }
	};*/
	meshData.Positions = {
		{ -width * 0.5f , -height * 0.5f,  0.0f }
		,{ -width * 0.5f,   height *0.5f,  0.0f }
		,{ width *0.5f ,  height *0.5f,  0.0f }
		,{ width *0.5f ,   -height * 0.5f,  0.0f }
	};


	meshData.Normals = {
		{ 0.f, 0.f, -1.0f }
		,{ 0.f, 0.f, -1.0f }
		,{ 0.f, 0.f, -1.0f }
		,{ 0.f, 0.f, -1.0f }
	};


	meshData.Indexes = { 0,1,2, 3,2,0 };
	return meshData;
}

engine::graphic::RawMeshData DemoUtils::CreateSphere(const float radius)
{
	
	engine::graphic::RawMeshData meshData;

	const unsigned int segments = 20;

	std::vector<float> thetaRange;
	std::vector<float> phiRange;
	thetaRange.reserve(segments + 1);
	phiRange.reserve(segments + 1);

	for (float currentTheta = 0, step = M_PI / segments; currentTheta < M_PI + 0.00001; currentTheta += step)
	{
		thetaRange.push_back(currentTheta);
	}

	for (float currentPhi = 0, step = M_PI * 2 / segments; currentPhi < M_PI * 2 + 0.00001; currentPhi += step)
	{
		phiRange.push_back(currentPhi);
	}

	unsigned int indexCounter = 0;
	for (int i = 0; i < thetaRange.size() - 1; ++i)
	{
		for (int j = 0; j < phiRange.size() - 1; ++j)
		{
			engine::graphic::Vec3f position;
			engine::graphic::Vec3f normal;


			computeSphereVertex(radius, thetaRange[i], phiRange[j], position, normal);
			meshData.Positions.push_back(position);
			meshData.Normals.push_back(normal);

			computeSphereVertex(radius, thetaRange[i + 1], phiRange[j], position, normal);
			meshData.Positions.push_back(position);
			meshData.Normals.push_back(normal);

			computeSphereVertex(radius, thetaRange[i + 1], phiRange[j + 1], position, normal);
			meshData.Positions.push_back(position);
			meshData.Normals.push_back(normal);

			computeSphereVertex(radius, thetaRange[i], phiRange[j + 1], position, normal);
			meshData.Positions.push_back(position);
			meshData.Normals.push_back(normal);


			meshData.Indexes.push_back(indexCounter + 0);
			meshData.Indexes.push_back(indexCounter + 1);
			meshData.Indexes.push_back(indexCounter + 2);

			meshData.Indexes.push_back(indexCounter + 3);
			meshData.Indexes.push_back(indexCounter + 2);
			meshData.Indexes.push_back(indexCounter + 0);

			indexCounter += 4;
		}
	}


	return meshData;

}
