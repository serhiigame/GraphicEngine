#include "DemoBase.h"

namespace GBufferBinding
{
	constexpr int Color = 0;
	constexpr int Position = 1;
	constexpr int Normal = 2;
	constexpr int Uv = 3;
}

void DemoBase::RegisterMaterials()
{
	
	engine::graphic::ShaderInfo GbuffShaderInfo;
	GbuffShaderInfo.vertShaderPath = "../res/shaders/GBuffer.vrt";
	GbuffShaderInfo.fragShaderPath = "../res/shaders/GBuffer.pxl";
	
	engine::graphic::ShaderOutputDesc gBuffInputColor;
	gBuffInputColor.Binding = GBufferBinding::Color;
	gBuffInputColor.Name = "g_color";
	
	engine::graphic::ShaderOutputDesc gBuffInputPos;
	gBuffInputPos.Binding = GBufferBinding::Position;
	gBuffInputPos.Name = "g_position";

	engine::graphic::ShaderOutputDesc gBuffInputNormal;
	gBuffInputNormal.Binding = GBufferBinding::Normal;
	gBuffInputNormal.Name = "g_normal";

	engine::graphic::ShaderOutputDesc gBuffInputUv;
	gBuffInputUv.Binding = GBufferBinding::Uv;
	gBuffInputUv.Name = "g_uv";

	GbuffShaderInfo.Outputs.push_back(gBuffInputColor);
	GbuffShaderInfo.Outputs.push_back(gBuffInputPos);
	GbuffShaderInfo.Outputs.push_back(gBuffInputNormal);
	GbuffShaderInfo.Outputs.push_back(gBuffInputUv);

	gApi.CreateGbuffer(GbuffShaderInfo);

	engine::graphic::ShaderInfo ShaderInfo;
	ShaderInfo.vertShaderPath = "../res/shaders/Diffuse.vrt";
	ShaderInfo.fragShaderPath = "../res/shaders/Diffuse.pxl";

	engine::graphic::ShaderInputDesc imputDesk;
	imputDesk.Binding = 11;
	imputDesk.Type = engine::graphic::EMaterialInputType::TEXTURE;
	imputDesk.Name = "param";

	ShaderInfo.Inputs.push_back(imputDesk);

	m_diffuseMaterial = gApi.RegisterMaterial(ShaderInfo);
}
