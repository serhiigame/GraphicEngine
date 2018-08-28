/*#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <stdio.h>

#include <algorithm>
#include <string>

#include <chrono>

#include "Graphic.h"

#include "../GU/ImageIO.h"

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	//glewExperimental = GL_TRUE;

	glfwSetErrorCallback(error_callback);
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
	glfwSetKeyCallback(window, key_callback);

	glewInit();

	GLint p[2];

	glGetIntegerv(GL_MAJOR_VERSION, p);
	glGetIntegerv(GL_MINOR_VERSION, p + 1);

	engine::graphic::Core core;
	engine::graphic::IApi * api = core.CreateApi(engine::graphic::EGApiType::OPEN_GL);

	//engine::graphic::IWindow apiWindow(window);
	//api->Init(&apiWindow);
	

	const GLfloat vertexes[] = {
		-0.8f, -0.8f,  0.0f,
		-0.8f, 0.8f,  0.0f,
		0.8f,  0.8f,  0.0f,
		0.8f, -0.8f,  0.0f
	};

	const GLfloat uv[] = {
		0.0f, 0.0f, 
		0.0f, 1.0f, 
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	
	const GLuint indexes[]{ 0,1,2, 3,2,0 };

	const GLfloat brightness = 0.5f;
	const GLfloat addColor[] = { 0.5f, 0.5f, 0.5f };

	const GLfloat texData[] = { 0.0f, 0.0f, 1.0f, 1.f };

	engine::graphic::IBuffer * vrt = api->CreateBuffer(sizeof(vertexes), engine::graphic::EBufferType::VERTEX);
	engine::graphic::IBuffer * uvc = api->CreateBuffer(sizeof(uv), engine::graphic::EBufferType::VERTEX);
	engine::graphic::IBuffer * idx = api->CreateBuffer(sizeof(indexes), engine::graphic::EBufferType::INDEX);
	engine::graphic::IConstant * cnst = api->CreateConatant(sizeof(brightness));
	engine::graphic::IConstant * acol = api->CreateConatant(sizeof(addColor));

	int w, h;
	const float * pData = engine::utility::ImageIo::Get().Load("../res/testImg.png", w, h);
	engine::utility::ImageIo::Get().Write("../res/testImgWrite.png", w, h, pData);

	engine::graphic::ITexture2D * tex = api->CreateTexture(w, h, engine::graphic::ETextureFormat::RGBA, engine::graphic::EDataType::FLOAT, engine::graphic::ETextureType::TEXTURE_2D);
	tex->Write(0, w, 0, h, pData);

	
	//engine::graphic::ITexture2D  * fbTex = api->CreateTexture(1, 1, engine::graphic::ETextureFormat::RGB, engine::graphic::EDataType::FLOAT, engine::graphic::ETextureType::TEXTURE_2D);
	//fbTex->Write(0, w, 0, h, pData);

	//engine::graphic::IFramebuffer * fb = api->CreateFramebuffer(640, 480);
	//fb->AttachTexture(fbTex);

	vrt->Write(0, sizeof(vertexes), vertexes);
	uvc->Write(0, sizeof(uv), uv);
	idx->Write(0, sizeof(indexes), indexes);
	cnst->Write(0, sizeof(brightness), &brightness);
	acol->Write(0, sizeof(addColor), &addColor);
	
	engine::graphic::IShader * shader = api->CreateShader("../res/shaders/test.vrt", "../res/shaders/test.pxl");
	shader->AttachAttribute(vrt, 0, 0, 3, engine::graphic::EDataType::FLOAT);
	shader->AttachAttribute(uvc, 1, 0, 2, engine::graphic::EDataType::FLOAT);
	shader->AttachAttribute(idx, 0, 0, 6, engine::graphic::EDataType::UNSIGNED_INT);
	shader->AttachConstant(cnst, 1);
	shader->AttachConstant(acol, 0);
	shader->AttachTexture(tex, 7);

	engine::graphic::IRenderPass * renderPass = api->CreateRenderPass();

	while (!glfwWindowShouldClose(window))
	{
		//glClearColor(1.0, 0.0, 0.0, 1); // black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//int a;
		//a = glGetError();

		renderPass->Execute(shader);

		//api->RenderPass(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
*/