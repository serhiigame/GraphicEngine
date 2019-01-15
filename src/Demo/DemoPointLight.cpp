#include "DemoPointLight.h"

void DemoPointLight::SetScene() {

	DemoUtils demoUtils(&gApi);
	DemoBase::SetScene();
	SetCameraDistance(0.9f);

	/*engine::Vec4f cameraTranslate({ 0.f, 0.0f, -0.9f, 1.f });
	//cameraTransform.SetTranslation(cameraTranslate);
	cameraTransform.SetRotation(gte::EulerAngles<float>(0, 1, 0, 0.f, 0.3f, 0.f));
	//cameraTransform.SetTranslation(cameraTranslate);
	cameraTransform.SetScale(1., 2., 1.);
	//cameraTransform.MakeIdentity();*/

	//gte::Matrix4x4<float> camereViewMatrix = cameraTransformTranslate.GetHMatrix() * cameraTransformRotete.GetHMatrix();

	//engine::graphic::RawMeshData rawMeshData = DemoUtils::CreateSphere(0.2);
	engine::graphic::RawMeshData rawMeshData = DemoUtils::CreatePlane(0.4f, 0.4f);

	/*std::vector<engine::graphic::RawMeshData> rawMeshDatas;
	if (!engine::utility::SceneIo::Get().LoadMesh("D:/Projects/1/FireRender/Resources/Obj/cube.obj", rawMeshDatas))
	{
	return;
	}

	engine::graphic::RawMeshData rawMeshData = rawMeshDatas[0];*/


	mesh = gApi.CreateMesh(rawMeshData);

	engine::graphic::MaterialInstance * material = gApi.CreateMaterialInstance(m_diffuseMaterial);

	engine::graphic::Texture2d * texture = demoUtils.LoadTexture2d("../res/images/uv.png");

	gApi.SetMaterialParameterTex2d(material, "param", texture);

	light = gApi.CreatePointLight();

	gApi.SetMeshMaterialInstance(mesh, material);

	gApi.SetPointLightPosition(light, engine::Vec3f({ 2.0f, 2.f, 5.0f }));

	gApi.SetPointLightIntensity(light, 1.);

	gApi.AddSceneMesh(m_scene, mesh);

	gApi.AddScenePointLight(m_scene, light);

}

void DemoPointLight::Update(double t)  {
	DemoBase::Update(t);
	//m_meshTranslate[2] += 0.0001f;
	//m_meshTransform.SetTranslation(m_meshTranslate);
	//cameraTranslate[2] = -10.f + sin(t);
	//cameraTransform.SetTranslation(cameraTranslate);
	//gApi.SetCameraView(camera, gte::Transpose(cameraTransform.GetHMatrix()));

	//meshTransform.MakeIdentity();
	//gApi.SetMeshTransform(mesh, gte::Transpose(m_meshTransform.GetHMatrix()));
	//gApi.SetMeshTransform(mesh1, gte::Transpose(transform.GetHMatrix()));
}