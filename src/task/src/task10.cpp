#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "task8.hpp"
#include "FileShader.hpp"
#include "CameraGL.hpp"

#include <stb_image.h>

#define STR(str) #str
#define STRING(str) STR(str)

namespace task10 {
    unsigned int loadTexture(const char *path) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;

        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

        GLenum format;
        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RED;
            break;
        }

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        return texture;
    }

    float mixValue = 0.2f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float lastX = 400;
    float lastY = 300;

    float pauseTime = 0.0f;
    bool isPause = false;

    bool firstMouse = true;

    CameraGL camera(glm::vec3(0.0f, 0.0f, 3.0f));

    glm::vec3 lightPos(1.2f, -0.3f, 2.0f);

    void processInput(GLFWwindow *window) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            mixValue = std::min(1.0f, mixValue + 0.001f);
        }

        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            mixValue = std::max(0.0f, mixValue - 0.001f);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            isPause = true;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            isPause = false;
        }
        //cameraPos.y = 0;
    }

    void mouse_callback(GLFWwindow *windows, double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera.processMouseMovement(xoffset, yoffset);
    }

    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        camera.processMouseScroll(yoffset);
    }

    // void setMat(GLint pos, glm::mat4 matrix) {
    //     glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(matrix));
    // }
}

int main10(int argc, char const *argv[])
{
    using namespace task10;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

     if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //std::string rootpath(STRING(SOURCE_ROOT));
    std::string resPath("resources");
    std::cout << resPath << std::endl;

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1 = loadTexture((resPath + "/texture" + "/container2.png").c_str());
    unsigned int texture2 = loadTexture((resPath + "/texture" + "/container2_specular.png").c_str());
    unsigned int texture3 = loadTexture((resPath + "/texture" + "/matrix.jpg").c_str());

    FileShader lightingShader(
         resPath + "/shader" + "/task10" + "/shader.vs",
         resPath + "/shader" + "/task10" + "/shader.fs");

    FileShader lightShader(
         resPath + "/shader" + "/task10" + "/shader_light.vs",
         resPath + "/shader" + "/task10" + "/shader_light.fs");

    float vertices[] = {
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f, 
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,

         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,

         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    };

    std::vector<glm::vec3> cubePositions {
        glm::vec3( 0.0f, -0.0f,  0.0f),
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);

    unsigned int lightVBO;
    glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);

    float radius = 10.0;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float rotatedAngle = 0.0f;
        if (isPause) {
            // if (!startPause) {
            //     lastPauseTime = currentFrame;
            //     startPause = true;
            // }
            pauseTime += deltaTime;
            
        } 
        rotatedAngle = (currentFrame - pauseTime) * 40;
        float radio = std::sqrt(lightPos.x * lightPos.x + lightPos.z * lightPos.z);
        float rotatedX = glm::cos(glm::radians(rotatedAngle)) * radio;
        float rotatedZ = glm::sin(glm::radians(rotatedAngle)) * radio;

        glm::vec3 lightColor;
        lightColor.x = sin(currentFrame * 4.0f * 2.0f) / 2.5f + (1.0f - 1.0f / 2.5f);
        lightColor.y = sin(currentFrame * 4.0f * 0.7f) / 2.5f + (1.0f - 1.0f / 2.5f);
        lightColor.z = sin(currentFrame * 4.0f * 1.3f) / 2.5f + (1.0f - 1.0f / 2.5f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = lightColor * glm::vec3(0.4f);

        // lightColor.x = 1.0f;
        // lightColor.y = 1.0f;
        // lightColor.z = 1.0f;
        // glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
        // glm::vec3 ambientColor = lightColor * glm::vec3(1.0f);
        

        lightingShader.use();
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        glUniform3f(lightingShader.getUniformLoc("objectColor"), 1.0f, 0.5f, 0.31f);
        const auto &cameraPos = camera.getCameraPos();
        glUniform3f(lightingShader.getUniformLoc("viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);

        lightingShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        lightingShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        lightingShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setVec3("light.position", rotatedX, lightPos.y, rotatedZ);
        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        glm::mat4 view;

        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;

        view = camera.getViewMatrix();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        // setMat(fileShader.getUniformLoc("view"), view);
        // setMat(fileShader.getUniformLoc("projection"), projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[0]);
        model = glm::rotate(model, (float) glfwGetTime() * 0.1f, {-1.0f, 1.0f, 0.0f});
        lightingShader.setMat4("model", model);

        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();
        glBindVertexArray(lightVAO);

        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("objectColor", lightColor);
        // setMat(lightShader.getUniformLoc("view"), view);
        // setMat(lightShader.getUniformLoc("projection"), projection);

        model = glm::mat4(1.0f);
        model = glm::translate(model, {rotatedX, lightPos.y, rotatedZ});
        model = glm::scale(model, glm::vec3 {0.2f, 0.2f, 0.2f});
        lightShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //glUseProgram(shaderProgram);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
