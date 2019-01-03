#include "DemoUtil.h"

#include "..\GU\ImageIO.h"
#include "..\GAPI\GApi.h"
#include "..\GAPI\Texture2d.h"
#include "..\GAPI\TextureCubeMap.h"

#define _USE_MATH_DEFINES
#include <math.h>


struct CubemapData {
	std::map<engine::graphic::ETextureCubeMapFace, size_t> DataFaceOffset;
	float * Data;
	float Width;
	float Height;
};



static void computeSphereVertex(float radius, float theta, float phi, engine::Vec3f & out_position, engine::Vec3f & out_normal)
{
	out_position = engine::Vec3f({ radius * sin(theta) * cos(phi) , radius * sin(theta) * sin(phi) , radius * cos(theta) });

	float len = sqrtf(out_position[0] * out_position[0] + out_position[1] * out_position[1] + out_position[2] * out_position[2]);
	out_normal = engine::Vec3f({ out_position[0] / len, out_position[1] / len, out_position[2] / len });
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

	meshData.Uv = {
		{ 0.f, 0.f }
		,{ 0.f, 1.f }
		,{ 1.f, 1.f }
		,{ 1.f, 0.f }
	};

	meshData.Indexes = { 0,1,2, 3,2,0 };
	return meshData;
}


void flipFaceImage4f(bool isFlipX, bool isFlipY, const size_t & size, float * data)
{
	const size_t bpp = 4;
	if (isFlipX)
	{
		for (int i = 0; i < size / 2; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				for (int k = 0; k < bpp; ++k)
				{
					std::swap(data[size * j * bpp + i * bpp + k], data[size * j * bpp + (size - i - 1) * bpp + k]);
				}
			}
		}
	}

	if (isFlipY)
	{
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size / 2; ++j)
			{
				for (int k = 0; k < bpp; ++k)
				{
					std::swap(data[size * j * bpp + i * bpp + k], data[size * (size - j - 1) * bpp + i * bpp + k]);
				}
			}
		}
	}
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
			engine::Vec3f position;
			engine::Vec3f normal;


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

engine::graphic::Texture2d * DemoUtils::LoadTexture2d(const std::string & path)
{
	int w, h;
	const int k_defaultBpp = 4;
	const float * pData = engine::utility::ImageIo::Get().Load(path, w, h);

	if (!pData)
	{
		return nullptr;
	}
	
	engine::graphic::Texture2d * texture2d = m_gapi->CreateTexture2d(w, h);

	texture2d->WriteImage(0, 0, w, h, pData);
	
	return texture2d;
}

engine::graphic::TextureCubeMap * DemoUtils::LoadCubeMap(const std::string & path)
{
	int w, h;
	const int k_defaultBpp = 4;
	const float * pData = engine::utility::ImageIo::Get().Load(path, w, h);

	if (!pData)
	{
		return nullptr;
	}

	const int wSectors = 4;
	const int hSectors = 3;
	

	int imgSize = w / wSectors;

	if (imgSize != h / hSectors)
	{
		return nullptr;
	}

	int faceIdxs[6][2] = {
		{0,1}
		,{ 1,0 }
		,{ 1,1 }
		,{ 1,2 }
		,{ 1,3 }
		,{ 2,1 }
	};

	engine::graphic::ETextureCubeMapFace faceOrder[6] =
	{
		engine::graphic::ETextureCubeMapFace::TOP
		, engine::graphic::ETextureCubeMapFace::LEFT
		, engine::graphic::ETextureCubeMapFace::BACK
		, engine::graphic::ETextureCubeMapFace::RIGHT
		, engine::graphic::ETextureCubeMapFace::FRONT
		, engine::graphic::ETextureCubeMapFace::BUTTOM
	};


	engine::graphic::TextureCubeMap * cubeMap = m_gapi->CreateTextureCubeMap(imgSize);
	std::vector<float> faceData(imgSize * imgSize  * k_defaultBpp);

	size_t offset = 0;
	for (int k = 0; k < 6; ++k)
	{
		for (int j = 0; j < imgSize; ++j)
		{
			size_t rawOffset = imgSize * k_defaultBpp;
			size_t hOffset = (faceIdxs[k][0] * imgSize + j) * (wSectors * imgSize * k_defaultBpp);
			size_t wOffset = faceIdxs[k][1] * imgSize * k_defaultBpp;

			memcpy(faceData.data() + offset, pData + hOffset + wOffset, imgSize * k_defaultBpp * sizeof(float));
			offset += rawOffset;
		}
		
		if (faceOrder[k] == engine::graphic::ETextureCubeMapFace::TOP
			|| faceOrder[k] == engine::graphic::ETextureCubeMapFace::BUTTOM)
		{
			flipFaceImage4f(false, true, imgSize, faceData.data());
		}
		else
		{
			flipFaceImage4f(true, false, imgSize, faceData.data());
		}
		
		cubeMap->WriteFace(faceOrder[k], faceData.data());

		offset = 0;
	}

	delete pData;

	return cubeMap;
}
