#pragma once
#include "../GAPI/RawMeshData.h"

class DemoUtils
{
public:
	static engine::graphic::RawMeshData CreatePlane(const float width, const float height);
	static engine::graphic::RawMeshData CreateSphere(const float radius);
};