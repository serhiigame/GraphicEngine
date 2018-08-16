#pragma once

struct GLFWwindow;
namespace engine
{
	namespace graphic
	{
		class IWindow
		{
		public:
			IWindow(GLFWwindow * w) : Window(w) {}
			unsigned int getWidth() const { return 640; }
			unsigned int getHeigth() const { return 480; }
			GLFWwindow * Window;
		};
	}
}