#include "DemoMaterial.h"

#include "../GAPI/Material.h"

engine::graphic::MaterialHandler g_diffuseMaterial;


void DemoMaterial::RegisterMaterials()
{
	
	engine::graphic::MaterialInfo materialInfo;
	materialInfo.vertShaderPath = "../res/shaders/Diffuse.vrt";
	materialInfo.fragShaderPath = "../res/shaders/Diffuse.pxl";

	engine::graphic::MaterialInputDesc imputDesk;
	imputDesk.Binding = 11;
	imputDesk.Type = engine::graphic::EMaterialInputType::TEXTURE;
	imputDesk.Name = "param";

	materialInfo.Descriptions.push_back(imputDesk);

	g_diffuseMaterial = gApi.RegisterMaterial(materialInfo);
}


void DemoMaterial::SetScene()  {

	DemoBase::SetScene();

	DemoUtils demoUtils(&gApi);

	RegisterMaterials();

	SetCameraDistance(0.9f);

	//engine::graphic::RawMeshData rawMeshData = DemoUtils::CreateSphere(0.2);
	engine::graphic::RawMeshData rawMeshData = DemoUtils::CreatePlane(0.5, 0.5);

	engine::graphic::TextureCubeMap * skybox = demoUtils.LoadCubeMap("../res/images/skybox.png");

	engine::graphic::Material * material = gApi.CreateMaterial(g_diffuseMaterial);

	engine::graphic::Texture2d * texture = demoUtils.LoadTexture2d("../res/images/uv.png");


	//gApi.SetMaterialParameterF4(material, "param", engine::Vec4f({ 0.f, 1.f, 0.f, 1.f }));

	gApi.SetMaterialParameterTex2d(material, "param", texture);

	mesh = gApi.CreateMesh(rawMeshData);

	gApi.SetMeshMaterial(mesh, material);

	light = gApi.CreatePointLight();

	gApi.SetPointLightPosition(light, engine::Vec3f({ 2.0f, 2.f, 5.0f }));

	gApi.SetPointLightIntensity(light, 1.);

	gApi.SetSceneSkybox(m_scene, skybox);

	gApi.AddSceneMesh(m_scene, mesh);

	gApi.AddScenePointLight(m_scene, light);
}

 void DemoMaterial::Update(double t)  {
	DemoBase::Update(t);


}
