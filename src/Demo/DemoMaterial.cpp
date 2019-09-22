#include "DemoMaterial.h"

#include "../GAPI/Material.h"


void DemoMaterial::SetScene()  {

	DemoBase::SetScene();

	DemoUtils demoUtils(&gApi);

	engine::Transform planeTransform;
	engine::Transform sphereTransform;

	planeTransform.SetRotation(gte::EulerAngles<float>(0, 1, 2, 0.f, M_PI_2, M_PI_2));
	sphereTransform.SetTranslation(engine::Vec4f({ 0.f, 0.2f, 0.f, 1.f }));

	SetCameraDistance(0.9f);

	engine::graphic::RawMeshData rawMeshSphere = DemoUtils::CreateSphere(0.2);
	engine::graphic::RawMeshData rawMeshPlane = DemoUtils::CreatePlane(5.0, 5.0);

	engine::graphic::GeTextureCubeMap skybox = demoUtils.LoadCubeMap("../res/images/skybox.png");

	engine::graphic::MaterialInstance * material1 = gApi.CreateMaterialInstance(m_diffuseMaterial);
	engine::graphic::GeTexture2d ballTexture = demoUtils.LoadTexture2d("../res/images/basketball.png");

	engine::graphic::MaterialInstance * material2 = gApi.CreateMaterialInstance(m_diffuseMaterial);
	engine::graphic::GeTexture2d grassTexture = demoUtils.LoadTexture2d("../res/images/grass.png");

	gApi.SetMaterialParameterTex2d(material1, "albedo", ballTexture);

	
	gApi.SetMaterialParameterTex2d(material2, "albedo", grassTexture);

	mesh1 = gApi.CreateMesh(rawMeshSphere);
	mesh2 = gApi.CreateMesh(rawMeshPlane);

	gApi.SetMeshMaterialInstance(mesh1, material1);
	gApi.SetMeshMaterialInstance(mesh2, material2);

	gApi.SetMeshTransform(mesh1, gte::Transpose(sphereTransform.GetHMatrix()));
	gApi.SetMeshTransform(mesh2, gte::Transpose(planeTransform.GetMatrix()));

	light = gApi.CreatePointLight();

	gApi.SetPointLightPosition(light, engine::Vec3f({ 2.0f, 2.f, 5.0f }));

	gApi.SetPointLightIntensity(light, 1.);

	//gApi.SetSceneSkybox(m_scene, skybox);

	gApi.AddSceneMesh(m_scene, mesh1);
	gApi.AddSceneMesh(m_scene, mesh2);

	gApi.AddScenePointLight(m_scene, light);
}

 void DemoMaterial::Update(double t)  {
	DemoBase::Update(t);
}
