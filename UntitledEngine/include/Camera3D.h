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

		void set_projectionMatrix(const glm::mat4 &_projectionMatrix) { Camera3D::_projectionMatrix = _projectionMatrix; }

		void set_viewMatrix(const glm::mat4 &_viewMatrix) { Camera3D::_viewMatrix = _viewMatrix; }

		void set_modelMatrix(const glm::mat4 &_modelMatrix) { Camera3D::_modelMatrix = _modelMatrix; }

		void set_mvpMatrix(const glm::mat4 &_mvpMatrix) { Camera3D::_mvpMatrix = _mvpMatrix; }

		//getters
		const glm::vec3 &getPosition() const { return _position; }

		float getScale() const { return _scale; }

		float getAspectRatio() const { return (float) _screenWidth / (float) _screenHeight; }

		const glm::mat4 &get_projectionMatrix() const { return _projectionMatrix; }

		const glm::mat4 &get_viewMatrix() const { return _viewMatrix; }

		const glm::mat4 &get_modelMatrix() const { return _modelMatrix; }

		const glm::mat4 &get_mvpMatrix() const { return _mvpMatrix; }

		const glm::mat4 &getCameraMatrix() const { return _cameraMatrix; }

	private:
		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		glm::vec3 _position;
		glm::vec3 _direction;
		glm::vec3 _up;

		glm::mat4 _cameraMatrix;
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _modelMatrix;
		glm::mat4 _mvpMatrix;
	};

}