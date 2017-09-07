#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace UntitledEngine {

	enum WindowFlags {
		INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4
	};

	class Window {
	public:
		Window();

		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		int create(GLFWwindow *win, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { return _screenWidth; }

		int getScreenHeight() { return _screenHeight; }

		GLFWwindow *getWindow() { return _window; }

	private:
		GLFWwindow *_window;
		int _screenWidth, _screenHeight;
	};

}