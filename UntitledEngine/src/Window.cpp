#include "Window.h"
#include "UntitledEngineErrors.h"

#include <GLFW/glfw3.h>

namespace UntitledEngine {

	Window::Window() {
	}


	Window::~Window() {
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

        //Uint32 flags = SDL_WINDOW_OPENGL;
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;

		(void)currentFlags;
//
//        if (currentFlags & INVISIBLE) {
//            flags |= SDL_WINDOW_HIDDEN;
//        }
//        if (currentFlags & FULLSCREEN) {
//            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
//        }
//        if (currentFlags & BORDERLESS) {
//            flags |= SDL_WINDOW_BORDERLESS;
//        }


		_window = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), NULL, NULL);
		if (_window == nullptr) {
			fatalError("GLFW Window could not be created!");
		}

		//Set up our OpenGL context
		glfwMakeContextCurrent(_window); // Initialize GLEW
		glewExperimental = true; // Needed in core profile

		//Set up glew
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize glew!");
		}

		//Check the OpenGL version
		std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

		//Set the background color to blue
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Set VSYNC
		glfwSwapInterval(0);

		// Enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		glfwSwapBuffers(_window);
	}

}