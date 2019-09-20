#include "DemoBase.h"
#include "DemoUtil.h"


namespace GBufferOutputBinding
{
	constexpr int Albedo = 0;
	constexpr int Position = 1;
	constexpr int Normal = 2;
	constexpr int Uv = 3;
	constexpr int MaterialId = 4;
}

namespace GBufferInputBinding
{
	constexpr int Albedo = 6;
}

namespace EnvMapInputBinding
{
	constexpr int Skybox = 10;
}

void DemoBase::RegisterMaterials()
{
	DemoUtils demoUtils(&gApi);

	// G-buffer

	engine::graphic::ShaderInfo GbuffShaderInfo;
	GbuffShaderInfo.vertShaderPath = "../res/shaders/GBuffer.vrt";
	GbuffShaderInfo.fragShaderPath = "../res/shaders/GBuffer.pxl";
	
	engine::graphic::ShaderOutputInfo gBuffOutputColor;
	gBuffOutputColor.Binding = GBufferOutputBinding::Albedo;
	gBuffOutputColor.Name = "g_albedo";
	gBuffOutputColor.DataType = engine::graphic::ETextureDataType::FLOAT;
	gBuffOutputColor.TextureFormat = engine::graphic::ETextureColorPack::RGBA;
	
	engine::graphic::ShaderOutputInfo gBuffOutputPos;
	gBuffOutputPos.Binding = GBufferOutputBinding::Position;
	gBuffOutputPos.Name = "g_position";
	gBuffOutputPos.DataType = engine::graphic::ETextureDataType::FLOAT;
	gBuffOutputPos.TextureFormat = engine::graphic::ETextureColorPack::RGBA;

	engine::graphic::ShaderOutputInfo gBuffOutputNormal;
	gBuffOutputNormal.Binding = GBufferOutputBinding::Normal;
	gBuffOutputNormal.Name = "g_normal";
	gBuffOutputNormal.DataType = engine::graphic::ETextureDataType::FLOAT;
	gBuffOutputNormal.TextureFormat = engine::graphic::ETextureColorPack::RGBA;

	engine::graphic::ShaderOutputInfo gBuffOutputUv;
	gBuffOutputUv.Binding = GBufferOutputBinding::Uv;
	gBuffOutputUv.Name = "g_uv";
	gBuffOutputUv.DataType = engine::graphic::ETextureDataType::FLOAT;
	gBuffOutputUv.TextureFormat = engine::graphic::ETextureColorPack::RGBA;

	engine::graphic::ShaderOutputInfo gBuffOutputMaterialId;
	gBuffOutputMaterialId.Binding = GBufferOutputBinding::MaterialId;
	gBuffOutputMaterialId.Name = "g_materialId";
	gBuffOutputMaterialId.DataType = engine::graphic::ETextureDataType::UNSIGNED_INT;
	gBuffOutputMaterialId.TextureFormat = engine::graphic::ETextureColorPack::RED;

	GbuffShaderInfo.Outputs.push_back(gBuffOutputColor);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputPos);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputNormal);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputUv);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputMaterialId);

	engine::graphic::GeTexture2d albedoFallback = demoUtils.LoadTexture2d("../res/images/gray.png");
	engine::graphic::ShaderInputInfoBasePtr gBuffInputAlbedo = engine::graphic::MaterialManager::MakeShaderTexture2dInputDesc(
		"albedo"
		, GBufferInputBinding::Albedo
		, albedoFallback);

	GbuffShaderInfo.Inputs.push_back(gBuffInputAlbedo);

	gApi.CreateGbuffer(GbuffShaderInfo);


	// Difuse Material
	engine::graphic::ShaderInfo ShaderInfo;
	ShaderInfo.vertShaderPath = "../res/shaders/Diffuse.vrt";
	ShaderInfo.fragShaderPath = "../res/shaders/Diffuse.pxl";

	m_diffuseMaterial = gApi.CreateMaterial(ShaderInfo);

	engine::graphic::MaterialInstance * materialInstance = gApi.CreateMaterialInstance(m_diffuseMaterial);
	gApi.SetDefaultMaterialInstance(materialInstance);


	// Environment Map
	engine::graphic::ShaderInfo envMapShaderInfo;

	envMapShaderInfo.vertShaderPath = "../res/shaders/Skybox.vrt";
	envMapShaderInfo.fragShaderPath = "../res/shaders/Skybox.pxl";

	engine::graphic::GeTextureCubeMap skyboxFallback = demoUtils.LoadCubeMap("../res/images/skybox.png");

	engine::graphic::ShaderInputInfoBasePtr envMapInputSkybox = engine::graphic::MaterialManager::MakeShaderTextureCubeMapInputDesc(
		"skybox"
		, EnvMapInputBinding::Skybox
		, skyboxFallback);

	envMapShaderInfo.Inputs.push_back(envMapInputSkybox);

	gApi.CreateEnvMap(envMapShaderInfo);

}
