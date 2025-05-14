#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera {
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 projectionMatrix, viewMatrix{1.0f}, modelMatrix;
	void update();
	void initialize();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	void setProjectionMatrix(glm::mat4 des);
	void setViewMatrix(glm::mat4 des);

};