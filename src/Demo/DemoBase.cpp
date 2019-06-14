#include "DemoBase.h"
#include "DemoUtil.h"


namespace GBufferOutputBinding
{
	constexpr int Albedo = 0;
	constexpr int Position = 1;
	constexpr int Normal = 2;
	constexpr int Uv = 3;
}

namespace GBufferInputBinding
{
	constexpr int Albedo = 6;
}

void DemoBase::RegisterMaterials()
{
	DemoUtils demoUtils(&gApi);

	engine::graphic::ShaderInfo GbuffShaderInfo;
	GbuffShaderInfo.vertShaderPath = "../res/shaders/GBuffer.vrt";
	GbuffShaderInfo.fragShaderPath = "../res/shaders/GBuffer.pxl";
	
	engine::graphic::ShaderOutputInfo gBuffOutputColor;
	gBuffOutputColor.Binding = GBufferOutputBinding::Albedo;
	gBuffOutputColor.Name = "g_albedo";
	
	engine::graphic::ShaderOutputInfo gBuffOutputPos;
	gBuffOutputPos.Binding = GBufferOutputBinding::Position;
	gBuffOutputPos.Name = "g_position";

	engine::graphic::ShaderOutputInfo gBuffOutputNormal;
	gBuffOutputNormal.Binding = GBufferOutputBinding::Normal;
	gBuffOutputNormal.Name = "g_normal";

	engine::graphic::ShaderOutputInfo gBuffOutputUv;
	gBuffOutputUv.Binding = GBufferOutputBinding::Uv;
	gBuffOutputUv.Name = "g_uv";

	GbuffShaderInfo.Outputs.push_back(gBuffOutputColor);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputPos);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputNormal);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputUv);

	engine::graphic::GeTexture2d albedoFallback = demoUtils.LoadTexture2d("../res/images/gray.png");
	engine::graphic::ShaderInputInfoBase * gBuffInputAlbedo = engine::graphic::MaterialManager::MakeShaderTextureInputDesc(
		"albedo"
		, GBufferInputBinding::Albedo
		, albedoFallback);

	GbuffShaderInfo.Inputs.push_back(gBuffInputAlbedo);

	gApi.CreateGbuffer(GbuffShaderInfo);

	engine::graphic::ShaderInfo ShaderInfo;
	ShaderInfo.vertShaderPath = "../res/shaders/Diffuse.vrt";
	ShaderInfo.fragShaderPath = "../res/shaders/Diffuse.pxl";

	m_diffuseMaterial = gApi.CreateMaterial(ShaderInfo);

	engine::graphic::MaterialInstance * materialInstance = gApi.CreateMaterialInstance(m_diffuseMaterial);
	gApi.SetDefaultMaterialInstance(materialInstance);

}
