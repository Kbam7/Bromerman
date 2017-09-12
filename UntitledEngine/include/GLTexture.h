#pragma once

#include <GL/glew.h>
#include <string>

namespace UntitledEngine {

	struct GLTexture {
		GLuint id;
		std::string type;
		std::string filePath = "";
		size_t width;
		size_t height;
	};
}