#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "../GAPI/GApi.h"
#include "../GAPI/Scene.h"

#include "Math.h"
#include "DemoUtil.h"

class DemoBase
{
public:

	virtual ~DemoBase() = 0 {}

	virtual void Init() {
	
		if (!glfwInit())
			exit(EXIT_FAILURE);

		// For vulkan
		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
		if (!m_window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(m_window);
		//glfwSetKeyCallback(window, key_callback);

		glewInit();

		gApi.Init();

		m_scene = gApi.CreateScene();

		SetScene();
	}

	void Run()
	{
		float t = 0.f;
		while (!glfwWindowShouldClose(m_window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Update(t);

			gApi.Render(m_scene);

			glfwSwapBuffers(m_window);
			glfwPollEvents();

			t += 0.0005;
		}

	}

	virtual void Deinit() {
		gApi.Deinit();

		glfwDestroyWindow(m_window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}


protected:
	virtual void SetScene() {}

	virtual void Update(double t) {}

protected:

	engine::graphic::GApi gApi;

	engine::graphic::Scene * m_scene;

private:
	GLFWwindow * m_window;
};

class PointLightAnim : public DemoBase
{
protected:
	virtual void SetScene() override {

		engine::Vec4f meshTranslate({ 0.0f, 0.0f, -0.0f, 1.f });
		meshTransform.SetTranslation(meshTranslate);

		engine::Transform cameraTransformTranslate;
		cameraTransformTranslate.SetTranslation(engine::Vec4f({ 0.f, 0.0f, -0.9f, 1.f }));

		engine::Transform cameraTransformRotete;
		cameraTransformRotete.SetRotation(gte::EulerAngles<float>(0, 1, 0, 0.f, 0.9f, 0.f));

		/*engine::Vec4f cameraTranslate({ 0.f, 0.0f, -0.9f, 1.f });
		//cameraTransform.SetTranslation(cameraTranslate);
		cameraTransform.SetRotation(gte::EulerAngles<float>(0, 1, 0, 0.f, 0.3f, 0.f));
		//cameraTransform.SetTranslation(cameraTranslate);
		cameraTransform.SetScale(1., 2., 1.);
		//cameraTransform.MakeIdentity();*/

		gte::Matrix4x4<float> camereViewMatrix = cameraTransformTranslate.GetHMatrix() * cameraTransformRotete.GetHMatrix();



		engine::Mat4f cameraProj = engine::MakeProjectionMatrix(1.f, 100, 90, 640. / 480);

		m_scene = gApi.CreateScene();


		engine::graphic::RawMeshData rawMeshData = DemoUtils::CreateSphere(0.2);
		//engine::graphic::RawMeshData rawMeshData = DemoUtils::CreatePlane(0.4f, 0.4f);

		mesh = gApi.CreateMesh(rawMeshData);

		camera = gApi.CreateCamera();

		light = gApi.CreatePointLight();

		gApi.SetCameraProjection(camera, cameraProj);

		gApi.SetCameraView(camera, gte::Transpose(camereViewMatrix));

		gApi.SetPointLightPosition(light, engine::Vec3f({ 0.0f, 0.f, -0.0f }));

		gApi.SetPointLightIntensity(light, 1.);

		gApi.SetSceneCamera(m_scene, camera);

		gApi.AddSceneMesh(m_scene, mesh);

		gApi.AddScenePointLight(m_scene, light);
	}

	virtual void Update(double t) override {
		//cameraTranslate[2] = -10.f + sin(t);
		//cameraTransform.SetTranslation(cameraTranslate);
		//gApi.SetCameraView(camera, gte::Transpose(cameraTransform.GetHMatrix()));

		//meshTransform.MakeIdentity();
		gApi.SetMeshTransform(mesh, gte::Transpose(meshTransform.GetHMatrix()));
		//gApi.SetMeshTransform(mesh1, gte::Transpose(transform.GetHMatrix()));
	}

private:
	engine::graphic::Mesh * mesh;
	engine::graphic::Camera * camera;
	engine::graphic::PointLight * light;

	engine::Transform meshTransform;
};



void main(void)
{
	
	DemoBase  * demo = new PointLightAnim();

	demo->Init();

	demo->Run();

	demo->Deinit();

	delete demo;
	/*
	//glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	// For vulkan
	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, key_callback);
	


	glewInit();
	
	
	engine::graphic::GApi gApi;
	gApi.Init();

	engine::Transform meshTransform;
	engine::Vec4f meshTranslate({ -0.0f, 0.0f, -0.9f, 1.f });
	meshTransform.SetTranslation(meshTranslate);

	engine::Transform cameraTransform;
	//engine::Vec4f cameraTranslate({ 0.f, -3.f, -7.f, 1.f });
	//cameraTransform.SetTranslation(cameraTranslate);

	cameraTransform.MakeIdentity();

	engine::Mat4f cameraProj = engine::MakeProjectionMatrix(1.f, 100, 90, 640. / 480);

	auto scene = gApi.CreateScene();


	engine::graphic::RawMeshData rawMeshData = DemoUtils::CreateSphere(0.2);
	//engine::graphic::RawMeshData rawMeshData = DemoUtils::CreatePlane(0.4f, 0.4f);

	auto mesh1 = gApi.CreateMesh(rawMeshData);

	auto camera = gApi.CreateCamera();

	auto light = gApi.CreatePointLight();

	gApi.SetCameraProjection(camera, cameraProj);

	gApi.SetCameraView(camera, gte::Transpose(cameraTransform.GetHMatrix()));

	gApi.SetPointLightPosition(light, engine::Vec3f({ 0.0f, 0.f, -0.0f }));

	gApi.SetPointLightIntensity(light, 1.);

	gApi.SetSceneCamera(scene, camera);

	gApi.AddSceneMesh(scene, mesh1);

	gApi.AddScenePointLight(scene, light);
	
	float t = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		t += 0.0005;
		//cameraTranslate[2] = -10.f + sin(t);
		//cameraTransform.SetTranslation(cameraTranslate);
		//gApi.SetCameraView(camera, gte::Transpose(cameraTransform.GetHMatrix()));

		//meshTransform.MakeIdentity();
		gApi.SetMeshTransform(mesh1, gte::Transpose(meshTransform.GetHMatrix()));
		//gApi.SetMeshTransform(mesh1, gte::Transpose(transform.GetHMatrix()));

		gApi.Render(scene);

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gApi.Deinit();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);*/
}