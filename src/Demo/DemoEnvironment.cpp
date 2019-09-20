#include "DemoEnvironment.h"

#include "DemoUtil.h"

void DemoEnvironment::SetScene()  {

	DemoBase::SetScene();

	SetCameraDistance(0.9f);

	engine::graphic::RawMeshData rawMeshData = DemoUtils::CreateSphere(0.2);

	DemoUtils demoUtils(&gApi);

	engine::graphic::GeTextureCubeMap skybox = demoUtils.LoadCubeMap("D:/Projects/GraphicEngine/res/Images/skybox.png");

	mesh = gApi.CreateMesh(rawMeshData);

	light = gApi.CreatePointLight();

	gApi.SetPointLightPosition(light, engine::Vec3f({ 2.0f, 2.f, 5.0f }));

	gApi.SetPointLightIntensity(light, 1.);

	//gApi.SetSceneSkybox(m_scene, skybox);

	gApi.AddSceneMesh(m_scene, mesh);

	gApi.AddScenePointLight(m_scene, light);
}

 void DemoEnvironment::Update(double t)  {
	DemoBase::Update(t);
}