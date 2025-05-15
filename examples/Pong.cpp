#include <source/Application.h>
#include <source/Shader.h>
#include <source/Camera.h>
#include <source/GLHelper.h>
const char* VERTEX_SHADER = R"(
#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
)";

const char* FRAGMENT_SHADER = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1,1,1,1);
}
)";
struct Vertex {
	float x, y, z;
};
class Pong :public Application {
public:
	Pong():Application("Pong","{ \n2 paddle 1 ball its Pong!\nWASD player 1 (left paddle)\nArrow Keys player 2 (right paddle) \n}",{800,800,"Pong"}) {}
	Shader shader{};
	Camera camera{};
	GLHelper::Batch batch;
	void initialize() {
		camera.initialize();
		shader.compile(VERTEX_SHADER,FRAGMENT_SHADER);
		batch.init();

		std::vector<GLHelper::__attrib> layout = {
			{0, 3, GL_FLOAT, GL_FALSE, 0},                           
		};
		batch.setLayout(layout);

		Vertex v1 = { -0.5f, -0.5f, 0.0f };
		Vertex v2 = { 0.5f, -0.5f, 0.0f};
		Vertex v3 = { -0.5f, 0.5f, 0.0f};

		Vertex v4 = { 0.5f, 0.5f, 0.0f };
		Vertex v5 = { 0.5f, -0.5f, 0.0f };
		Vertex v6 = { -0.5f, 0.5f, 0.0f };

		batch.submitVertex(&v1);
		batch.submitVertex(&v2);
		batch.submitVertex(&v3);

		batch.submitVertex(&v4);
		batch.submitVertex(&v5);
		batch.submitVertex(&v6);

		batch.upload();
		std::function<void(const KeyEvent&)>event = { [this](const KeyEvent& event) {
			if (event.keyCode == GLFW_KEY_ESCAPE && event.actionCode == GLFW_PRESS)
				setState(AppState::EXIT);
			if (event.keyCode == GLFW_KEY_W && event.actionCode == GLFW_PRESS)
				w1vy = 0.5f;
			if (event.keyCode == GLFW_KEY_S && event.actionCode == GLFW_PRESS)
				w1vy = -0.5f;
			if (event.keyCode == GLFW_KEY_UP && event.actionCode == GLFW_PRESS)
				w2vy = 0.5f;
			if (event.keyCode == GLFW_KEY_DOWN && event.actionCode == GLFW_PRESS)
				w2vy = -0.5f;
			}};
		this->subscribe(event);

	}
	int w1p = 0, w2p = 0;
	const float friction = 0.05f;
	const float BALL_SIZE = 0.1f*0.5f;
	const float PADDLE_WIDTH = 0.1f*0.5f;
	const float PADDLE_HEIGHT = 0.4f*0.5f;
	float w1y = 0, w2y = 0;
	float w1x = -1.5f, w2x = 1.5f;
	float w1vy = 0,w2vy = 0;
	float ballx = 0, bally = 0;
	float xvelocity = 0.1f, yvelocity = 0.01f;
	void update(double dt) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (glfwWindowShouldClose(getGLFWwindow())) {
			setState(AppState::EXIT);
		}
		setStatus("ballx:: " + std::to_string(ballx));
		ballx += xvelocity * dt*10;
		bally += yvelocity * dt*10;

		if (w1y >= 1.0f) {
			w1y = 1.0f;
			w1vy *= -1.0f;
		}
		if (w1y <=-1.0f) {
			w1y = -1.0f;
			w1vy *= -1.0f;
		}

		if (w2y >= 1.0f) {
			w2y = 1.0f;
			w2vy *= -1.0f;
		}
		if (w2y <= -1.0f) {
			w2y = -1.0f;
			w2vy *= -1.0f;
		}
		    
		w1y += w1vy * dt * 2;
		w2y += w2vy * dt * 2;
		


		float ballLeft = ballx - BALL_SIZE;
		float ballRight = ballx + BALL_SIZE;
		float ballTop = bally + BALL_SIZE;
		float ballBottom = bally - BALL_SIZE;

		float paddle1Left = w1x - PADDLE_WIDTH;
		float paddle1Right = w1x + PADDLE_WIDTH;
		float paddle1Top = w1y + PADDLE_HEIGHT;
		float paddle1Bottom = w1y - PADDLE_HEIGHT;

		float paddle2Left = w2x - PADDLE_WIDTH;
		float paddle2Right = w2x + PADDLE_WIDTH;
		float paddle2Top = w2y + PADDLE_HEIGHT;
		float paddle2Bottom = w2y - PADDLE_HEIGHT;

		bool collidesWithPaddle1 =
			ballRight >= paddle1Left &&
			ballLeft <= paddle1Right &&
			ballTop >= paddle1Bottom &&
			ballBottom <= paddle1Top;

		bool collidesWithPaddle2 =
			ballRight >= paddle2Left &&
			ballLeft <= paddle2Right &&
			ballTop >= paddle2Bottom &&
			ballBottom <= paddle2Top;

		if (collidesWithPaddle1 || collidesWithPaddle2) {
			float paddleY = collidesWithPaddle1 ? w1y : w2y;

			float offset = (bally - paddleY) / PADDLE_HEIGHT;
			if (offset == 0) {
				addLog("offset is zero!");
			}
			yvelocity = offset * 0.03f; 
			xvelocity *= -1.15f;
		}
		if (bally > 1.0f ||bally<-1.0f) {
			yvelocity *= -1.0f;
		}
		if (ballx > 1.5f) {
			xvelocity = 0.1f;
			ballx = 0.0f;
			bally = 0.0f;
			w1p++;
		}
		if (ballx < -1.5f) {
			xvelocity = -0.1f;
			ballx = 0.0f;
			bally = 0.0f;
			w2p++;
		}

		setStatus("player1:: "+std::to_string(w1p)+" \n"+ "player2:: "+std::to_string(w2p));

		shader.bind();
		shader.uniform_mat4("projection", camera.getProjectionMatrix());
		shader.uniform_mat4("view", camera.getViewMatrix());
		
		
		glm::mat4 currentBallModel = glm::translate(glm::mat4(1.0f), glm::vec3(ballx, bally, 0.0f));
		currentBallModel = glm::scale(currentBallModel, glm::vec3(0.1f, 0.1f, 0.0f));
		shader.uniform_mat4("model", currentBallModel);
		batch.draw();

		glm::mat4 wall1model = glm::translate(glm::mat4(1.0f), glm::vec3(w1x, w1y, 0.0f));
		wall1model = glm::scale(wall1model, glm::vec3(0.1f,0.4f,0.0f));
		shader.uniform_mat4("model", wall1model);
		batch.draw();

		glm::mat4 wall2model = glm::translate(glm::mat4(1.0f), glm::vec3(w2x, w2y, 0.0f));
		wall2model = glm::scale(wall2model, glm::vec3(0.1f, 0.4f, 0.0f));
		shader.uniform_mat4("model", wall2model);
		batch.draw();

		shader.unbind();
		camera.update();

		glfwSwapBuffers(getGLFWwindow());
		glfwPollEvents();
	}
	void destroy() {
		addLog("player1:: " + std::to_string(w1p));
		addLog("player2:: " + std::to_string(w2p));
		batch.dispose();
	}
};

int main() {
	Pong Pong{};
	Pong.start();
	return 0;
}