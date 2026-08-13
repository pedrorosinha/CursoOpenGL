#include "Camera.h"

// ========================================================
// CAMERA
Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

// ========================================================
// Atualiza a matriz de projeção e view no shader.
void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

// ========================================================
// Passa a matriz da câmera para o shader.
void Camera::Matrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

// ========================================================
// Atualiza a posição da câmera com base nos inputs do usuário.
void Camera::Inputs(GLFWwindow* window, float deltaTime) {
	float velocity = speed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += velocity * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position += velocity * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += velocity * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += velocity * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Position += velocity * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Position += velocity * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = 0.1f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		glm::vec3 newOrientation = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up))) * glm::vec4(Orientation, 1.0f));

		float angToUp = glm::acos(glm::clamp(glm::dot(glm::normalize(newOrientation), glm::normalize(Up)), -1.0f, 1.0f));
		float angToDown = glm::acos(glm::clamp(glm::dot(glm::normalize(newOrientation), glm::normalize(-Up)), -1.0f, 1.0f));

		if (angToUp <= glm::radians(5.0f) && angToDown <= glm::radians(5.0f)) {
			Orientation = newOrientation;
		}

		Orientation = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotY), Up) * glm::vec4(Orientation, 1.0f));

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}