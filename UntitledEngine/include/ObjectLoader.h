#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <GLM/glm.hpp>
#include <vector>

namespace UntitledEngine {
bool loadAssImp(
		const char * path,
		std::vector<unsigned short> & indices,
		std::vector<glm::vec3> & vertices,
		std::vector<glm::vec2> & uvs,
		std::vector<glm::vec3> & normals
);
}
#endif //OBJECTLOADER_H
