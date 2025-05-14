#pragma once

#include <source/Application.h>
#include <source/Shader.h>
#include <source/Texture.h>
#include <source/Camera.h>

const char* VERTEX_SHADER = R"(
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
out vec2 fragTexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    fragTexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
)";

const char* FRAGMENT_SHADER = R"(
#version 330 core
in vec2 fragTexCoord;
out vec4 FragColor;
uniform sampler2D texture1;
void main()
{
    FragColor = texture(texture1, fragTexCoord);
}
)";

float vertices[] = {
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
};


GLuint indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9,10,10,11, 8,
   12,13,14,14,15,12,
   16,17,18,18,19,16,
   20,21,22,22,23,20
};


class TestApp1 : public Application {
public:
    Shader shader{};
    Texture texture{ "img1.png" };
    Camera cam;
    GLuint vao, vbo, ebo;

    TestApp1() : Application("TestApp1", "3D Cubes and 3D Camera Example", { 800, 600, "3D Cube" }) {}
    std::string status;
    void initialize() {
        cam.initialize();
        shader.compile(VERTEX_SHADER, FRAGMENT_SHADER);
        addLog(texture.create());

        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
    void add(std::string msg, float val) {
        status += msg;
        status += " :: ";
        status += std::to_string(val);
        status += "\n";
    }
    float yaw = -90.0f, pitch = 0.0f;
    void updateCamera(float deltaTime, Camera& cam) {
        float cameraSpeed = 2.5f * deltaTime;
        add("camSpeed:: ", cameraSpeed);
        float camyp_app = 20.0f;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
            yaw -= cameraSpeed * camyp_app;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
            yaw += cameraSpeed * camyp_app;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS)
            pitch += cameraSpeed * camyp_app;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
            pitch -= cameraSpeed * camyp_app;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        add("yaw", yaw);
        add("pitch", pitch);
        add("camPosX", cam.cameraPos.x);
        add("camPosY", cam.cameraPos.y);
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cam.cameraFront = glm::normalize(direction);

        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
            cam.cameraPos += cameraSpeed * cam.cameraFront;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
            cam.cameraPos -= cameraSpeed * cam.cameraFront;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
            cam.cameraPos -= glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
        if (glfwGetKey(getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
            cam.cameraPos += glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
    }
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    void update(double delta) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwWindowShouldClose(getGLFWwindow())) {
            setState(AppState::EXIT);
        }

        glm::mat4 model = glm::mat4(1.0f);


        shader.bind();

        shader.uniform_mat4("projection", cam.getProjectionMatrix());
        shader.uniform_mat4("view", cam.getViewMatrix());

        texture.bindDefault();
        for (size_t i = 0; i < 10; i++)
        {
            model = glm::translate(model, glm::vec3(1.0f + i, 0.0f, 0.0f));
            shader.uniform_mat4("model", model);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        texture.unbind();

        shader.unbind();

        GLenum glerr = glGetError();
        if (glerr != GL_NO_ERROR) {
            addLog("OGL:[ERROR]: " + std::to_string(glerr));
        }
        updateCamera(delta, cam);
        cam.update();
        setStatus(status);
        status.clear();
        glfwSwapBuffers(getGLFWwindow());
        glfwPollEvents();
    }

    void destroy() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
};

int main() {
    TestApp1 app;
    app.start();
    return 0;
}