#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "../GAPI/GApi.h"
#include "../GAPI/Scene.h"

#include "Math.h"
#include "DemoUtil.h"

#include "../GU/SceneIO.h"

#include <set>


class DemoBase;
DemoBase * g_demoBase = nullptr;


class IInputEventListener
{
public:
	virtual void MouseWheelEvent(double value) {}
	virtual void MouseDragEvent(int button, double x, double y) {}
};

class InputEventManager
{
public:
	static InputEventManager & Get()
	{
		static InputEventManager managet;
		return managet;
	}


	void RegisterGlfwEvents(GLFWwindow * window)
	{
		glfwSetScrollCallback(window, InputEventManager::MouseScrollCallback);
		glfwSetMouseButtonCallback(window, InputEventManager::MouseButtonfunCallback);
		glfwSetCursorPosCallback(window, InputEventManager::MouseCursorPosCallback);
	}

	void AddListener(IInputEventListener * lidtener)
	{
		if (lidtener)
		{
			m_listeners.insert(lidtener);
		}
	}

	void RemoveListener(IInputEventListener * lidtener)
	{
		if (lidtener && m_listeners.find(lidtener) != m_listeners.end())
		{
			m_listeners.erase(lidtener);
		}
	}

private:

	static void  MouseScrollCallback(GLFWwindow *, double x, double y)
	{
		constexpr const float ScrollSpeed = 0.1f;
		InputEventManager & self = InputEventManager::Get();


		for (auto listener : self.m_listeners)
		{
			listener->MouseWheelEvent(y);
		}
	}

	static void MouseButtonfunCallback(GLFWwindow *, int button, int action, int mods)
	{
		InputEventManager & self = InputEventManager::Get();
		
		if (GLFW_MOUSE_BUTTON_LEFT == button)
		{
			self.m_MouseLeftButtonStatus = action;
		}

		if (GLFW_MOUSE_BUTTON_MIDDLE == button)
		{
			self.m_MouseMiddleButtonStatus = action;
		}

		if (GLFW_MOUSE_BUTTON_MIDDLE == button)
		{
			self.m_MouseRightButtonStatus = action;
		}

		self.m_MouseModifierKey = mods;
	}

	static void MouseCursorPosCallback(GLFWwindow *, double x, double y)
	{
		InputEventManager & self = InputEventManager::Get();
		
		double dx = x - self.m_mousePosCurrentX;
		double dy = y - self.m_mousePosCurrentY;

		self.m_mousePosCurrentX = x;
		self.m_mousePosCurrentY = y;

		if (self.m_MouseMiddleButtonStatus == GLFW_PRESS)
		{
			for (auto listener : self.m_listeners)
			{
				listener->MouseDragEvent(GLFW_MOUSE_BUTTON_MIDDLE, dx, dy);
			}
		}
	}

	int m_MouseLeftButtonStatus = GLFW_RELEASE;
	int m_MouseMiddleButtonStatus = GLFW_RELEASE;
	int m_MouseRightButtonStatus = GLFW_RELEASE;
	int m_MouseModifierKey = NULL;

	double m_mousePosCurrentX = 0.;
	double m_mousePosCurrentY = 0.;


	std::set<IInputEventListener *> m_listeners;
};



class DemoBase : public IInputEventListener
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
		InputEventManager & eventManager = InputEventManager::Get();
		eventManager.RegisterGlfwEvents(m_window);
		eventManager.AddListener(this);

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

		InputEventManager & eventManager = InputEventManager::Get();
		eventManager.RemoveListener(this);

		glfwDestroyWindow(m_window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}


protected:
	virtual void SetScene() {

		m_scene = gApi.CreateScene();
		m_camera = gApi.CreateCamera();
		
		engine::Mat4f cameraProj = engine::MakeProjectionMatrix(1.f, 100, 90, 640. / 480);

		gApi.SetCameraProjection(m_camera, cameraProj);

		gApi.SetSceneCamera(m_scene, m_camera);

	}

	virtual void Update(double t) {
		UpdateCamera(t);
	}

protected:

	engine::graphic::GApi gApi;

	engine::graphic::Scene * m_scene;

	void SetCameraDistance(const float & distance)
	{
		m_cameraDistance = distance;
		g_demoBase->m_isCamereDirty = true;
	}

private:

	virtual void  MouseWheelEvent(double value) override
	{
		constexpr const float ScrollSpeed = 0.1f;

		g_demoBase->m_cameraDistance -= value * ScrollSpeed;
		g_demoBase->m_isCamereDirty = true;
	}

	virtual void MouseDragEvent(int button, double x, double y) override
	{
		const float rorationSpeed = 0.01f;
		
		m_cameraYaw += float(x) * rorationSpeed;
		m_cameraPitch += float(y) * rorationSpeed;

		g_demoBase->m_isCamereDirty = true;
	}

	void UpdateCamera(double t)
	{
		if (m_isCamereDirty)
		{
			
			engine::Mat4f cameraTransform;
			cameraTransform.MakeIdentity();

			const float sqrtFloatEps = sqrtf(FLT_EPSILON);
			if (fabs(m_cameraDistance) > sqrtFloatEps)
			{
				engine::Transform cameraTransformTranslate;
				cameraTransformTranslate.SetTranslation(engine::Vec4f({ 0.f, 0.0f, -m_cameraDistance, 1.f }));
				cameraTransform = cameraTransform * cameraTransformTranslate.GetHMatrix();
			}
			if (fabs(m_cameraYaw) > sqrtFloatEps || fabs(m_cameraPitch) > sqrtFloatEps)
			{
				engine::Transform cameraTransformRotete;
				cameraTransformRotete.SetRotation(gte::EulerAngles<float>(1, 0, 2, m_cameraYaw, m_cameraPitch, 0.f));

				cameraTransform = cameraTransform * cameraTransformRotete.GetHMatrix();
			}

			gApi.SetCameraView(m_camera, gte::Transpose(cameraTransform));
			
			m_isCamereDirty = false;
		}
	}



	GLFWwindow * m_window;


	engine::graphic::Camera * m_camera = nullptr;

	bool m_isCamereDirty = true;

	float m_cameraDistance = 0.0f;
	float m_cameraYaw = 0.0f;
	float m_cameraPitch = 0.0f;

	engine::Vec4f m_cameraLookAt; // TODO
};

class PointLightAnim : public DemoBase
{
protected:
	virtual void SetScene() override {

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
		//engine::graphic::RawMeshData rawMeshData = DemoUtils::CreatePlane(0.4f, 0.4f);

		std::vector<engine::graphic::RawMeshData> rawMeshDatas;
		if (!engine::utility::SceneIo::Get().LoadMesh("D:/Projects/1/FireRender/Resources/Obj/cube.obj", rawMeshDatas))
		{
			return;
		}

		engine::graphic::RawMeshData rawMeshData = rawMeshDatas[0];

		mesh = gApi.CreateMesh(rawMeshData);

		light = gApi.CreatePointLight();

		gApi.SetPointLightPosition(light, engine::Vec3f({ 0.0f, 0.f, -0.0f }));

		gApi.SetPointLightIntensity(light, 1.);

		gApi.AddSceneMesh(m_scene, mesh);

		gApi.AddScenePointLight(m_scene, light);
	}

	virtual void Update(double t) override {
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

private:
	engine::graphic::Mesh * mesh = nullptr;
	engine::graphic::PointLight * light = nullptr;
};



void main(void)
{
	DemoBase  * demo = new PointLightAnim();
	g_demoBase = demo;

	demo->Init();
	demo->Run();
	demo->Deinit();

	delete demo;
	g_demoBase = nullptr;
	
}