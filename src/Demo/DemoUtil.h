#pragma once
#include <map>

#include "../LLR/LLREnum.h"

// DELETE
#include "../GAPI/GApi.h"

namespace engine
{
	namespace graphic
	{
		class GApi;
		class Texture2d;
		class TextureCubeMap;
	}
}

class DemoUtils final
{
public:
	DemoUtils(engine::graphic::GApi * gApi) : m_gapi(gApi) {}

	static engine::graphic::RawMeshData CreatePlane(const float width, const float height);
	static engine::graphic::RawMeshData CreateSphere(const float radius);

	engine::graphic::Texture2d * LoadTexture2d(const std::string & path);

	engine::graphic::TextureCubeMap * LoadCubeMap(const std::string & path);

private:

	engine::graphic::GApi * m_gapi;
};