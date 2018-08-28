#pragma once

#include <set>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>


#include "../GAPI/GApi.h"
#include "../GAPI/Scene.h"

#include "Math.h"

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
		m_isCamereDirty = true;
	}

private:

	virtual void  MouseWheelEvent(double value) override
	{
		constexpr const float ScrollSpeed = 0.1f;

		m_cameraDistance -= value * ScrollSpeed;
		m_isCamereDirty = true;
	}

	virtual void MouseDragEvent(int button, double x, double y) override
	{
		const float rorationSpeed = 0.01f;

		m_cameraYaw += float(x) * rorationSpeed;
		m_cameraPitch += float(y) * rorationSpeed;

		m_isCamereDirty = true;
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
