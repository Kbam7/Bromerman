#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "UntitledEngine.h"
#include "UntitledEngineErrors.h"

namespace UntitledEngine {

	int init() {

		// Initialise GLFW
		if (!glfwInit()) {
			fatalError("Failed to initialize GLFW\n");
			return -1;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);                        // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);          // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

		return 0;
	}

}