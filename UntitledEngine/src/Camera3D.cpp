#include "Camera3D.h"

namespace UntitledEngine {

	Camera3D::Camera3D() :
			_screenWidth(500),
			_screenHeight(500),
			_needsMatrixUpdate(true),
			_scale(1.0f),
			_position(0.0f, 0.0f, 0.0f),
			_cameraMatrix(1.0f),
			_mvpMatrix(1.0f) {
	}

	Camera3D::~Camera3D() {
	}

	void Camera3D::init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		//_mvpMatrix = glm::ortho(0.0f, (float) _screenWidth, 0.0f, (float) _screenHeight);

		// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

		// Camera matrix
		glm::mat4 View = glm::lookAt(
				glm::vec3(0,0,-5), // Camera is at (4,3,3), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(1,0,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		_mvpMatrix = Projection * View * Model; // Remember, matrix multiplication is the other way around*/
	}

	//updates the camera matrix if needed
	void Camera3D::update() {

		//Only update if our position or scale have changed
		if (_needsMatrixUpdate) {

			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, -_position.y + _screenHeight / 2);
			_cameraMatrix = glm::translate(_mvpMatrix, translate);

			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
	}

	glm::vec3 Camera3D::convertScreenToWorld(glm::vec3 screenCoords) {
		// Invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		// Make it so that 0 is the center
		screenCoords -= glm::vec3(_screenWidth / 2, _screenHeight / 2, 0);
		// Scale the coordinates
		screenCoords /= _scale;
		// Translate with the camera position
		screenCoords += _position;
		return screenCoords;
	}

	// Simple AABB test to see if a box is in the camera view
	bool Camera3D::isBoxInView(const glm::vec3 &position, const glm::vec2 &dimensions) {

		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);

		// The minimum distance before a collision occurs
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		// Center position of the parameters
		glm::vec3 centerPos = position + glm::vec3(dimensions, 0.0f) / 2.0f;
		// Center position of the camera
		glm::vec3 centerCameraPos = _position;
		// Vector from the input to the camera
		glm::vec3 distVec = centerPos - centerCameraPos;

		// Get the depth of the collision
		float xDepth = MIN_DISTANCE_X - std::abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - std::abs(distVec.y);

		// If both the depths are > 0, then we collided
		if (xDepth > 0 && yDepth > 0) {
			// There was a collision
			return true;
		}
		return false;
	}
}