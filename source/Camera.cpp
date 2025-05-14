#include "Camera.h"
void Camera::update() {
	Camera::viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
void Camera::initialize() {
	viewMatrix = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
	projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
		100.0f);
}

glm::mat4 Camera::getProjectionMatrix(){
return projectionMatrix;
};
glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
};
void Camera::setProjectionMatrix(glm::mat4 des) {
	projectionMatrix = des;
};
void Camera::setViewMatrix(glm::mat4 des) {
	viewMatrix = des;
};