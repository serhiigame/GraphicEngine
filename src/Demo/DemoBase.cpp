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

	engine::graphic::ShaderDesc GbuffShaderInfo;
	GbuffShaderInfo.vertShaderPath = "../res/shaders/GBuffer.vrt";
	GbuffShaderInfo.fragShaderPath = "../res/shaders/GBuffer.pxl";
	
	engine::graphic::ShaderOutputDesc gBuffOutputColor;
	gBuffOutputColor.Binding = GBufferOutputBinding::Albedo;
	gBuffOutputColor.Name = "g_albedo";
	
	engine::graphic::ShaderOutputDesc gBuffOutputPos;
	gBuffOutputPos.Binding = GBufferOutputBinding::Position;
	gBuffOutputPos.Name = "g_position";

	engine::graphic::ShaderOutputDesc gBuffOutputNormal;
	gBuffOutputNormal.Binding = GBufferOutputBinding::Normal;
	gBuffOutputNormal.Name = "g_normal";

	engine::graphic::ShaderOutputDesc gBuffOutputUv;
	gBuffOutputUv.Binding = GBufferOutputBinding::Uv;
	gBuffOutputUv.Name = "g_uv";

	GbuffShaderInfo.Outputs.push_back(gBuffOutputColor);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputPos);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputNormal);
	GbuffShaderInfo.Outputs.push_back(gBuffOutputUv);

	engine::graphic::GeTexture2d albedoFallback = demoUtils.LoadTexture2d("../res/images/uv.png");
	engine::graphic::ShaderInputDesc gBuffInputAlbedo = engine::graphic::MaterialHelper::MakeShaderTextureInputDesc(
		"albedo"
		, GBufferInputBinding::Albedo
		, albedoFallback);
	//gBuffInputAlbedo.Type = engine::graphic::EMaterialInputType::TEXTURE;
	//gBuffInputAlbedo.Binding = GBufferInputBinding::Albedo;
	//gBuffInputAlbedo.Name = "albedo";

	GbuffShaderInfo.Inputs.push_back(gBuffInputAlbedo);

	gApi.CreateGbuffer(GbuffShaderInfo);

	engine::graphic::ShaderDesc ShaderInfo;
	ShaderInfo.vertShaderPath = "../res/shaders/Diffuse.vrt";
	ShaderInfo.fragShaderPath = "../res/shaders/Diffuse.pxl";

	//engine::graphic::ShaderInputDesc imputDesk;
	//imputDesk.Binding = 11;
	//imputDesk.Type = engine::graphic::EMaterialInputType::TEXTURE;
	//imputDesk.Name = "param";

	//ShaderInfo.Inputs.push_back(imputDesk);

	m_diffuseMaterial = gApi.CreateMaterial(ShaderInfo);
}
