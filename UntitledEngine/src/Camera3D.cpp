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
		glm::mat4 _projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

		// Camera matrix
		glm::mat4 _viewMatrix = glm::lookAt(
				glm::vec3(0,0,0), // Position
				glm::vec3(0,0,0), // and looks at this point
				glm::vec3(1,0,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 _modelMatrix = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		_mvpMatrix = _projectionMatrix * _viewMatrix * _modelMatrix; // Remember, matrix multiplication is the other way around*/
	}

	//updates the camera matrix if needed
	void Camera3D::update() {

		//Only update if our position or scale have changed
		if (_needsMatrixUpdate) {

			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, _position.z);
			_cameraMatrix = glm::translate(_mvpMatrix, translate);

			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
	}

}