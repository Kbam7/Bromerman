#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace UntitledEngine {

	//Camera class for 2D games
	class Camera3D {
	public:
		Camera3D();

		~Camera3D();

		//sets up the orthographic matrix and screen dimensions
		void init(int screenWidth, int screenHeight);

		//updates the camera matrix if needed
		void update();

		glm::vec3 convertScreenToWorld(glm::vec3 screenCoords);

		bool isBoxInView(const glm::vec3 &position, const glm::vec2 &dimensions);

		void offsetPosition(const glm::vec3 &offset) {
			_position += offset;
			_needsMatrixUpdate = true;
		}

		void offsetScale(float offset) {
			_scale += offset;
			if (_scale < 0.001f) _scale = 0.001f;
			_needsMatrixUpdate = true;
		}

		//setters
		void setPosition(const glm::vec3 &newPosition) {
			_position = newPosition;
			_needsMatrixUpdate = true;
		}

		void setScale(float newScale) {
			_scale = newScale;
			_needsMatrixUpdate = true;
		}

		//getters
		const glm::vec3 &getPosition() const { return _position; }

		float getScale() const { return _scale; }

		const glm::mat4 &getCameraMatrix() const { return _cameraMatrix; }

		float getAspectRatio() const { return (float) _screenWidth / (float) _screenHeight; }

	private:
		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		glm::vec3 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _mvpMatrix;
	};

}