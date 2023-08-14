#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "task12.hpp"
#include "FileShader.hpp"
#include "CameraGL.hpp"
#include "Model.hpp"

#include <stb_image.h>

#define STR(str) #str
#define STRING(str) STR(str)

namespace task12 {
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

        // if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        //     mixValue = std::min(1.0f, mixValue + 0.001f);
        // }

        // if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        //     mixValue = std::max(0.0f, mixValue - 0.001f);
        // }

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

int main12(int argc, char const *argv[])
{
    using namespace task12;

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
         resPath + "/shader" + "/task12" + "/shader.vs",
         resPath + "/shader" + "/task12" + "/shader.fs");

    FileShader lightShader(
         resPath + "/shader" + "/task12" + "/shader_light.vs",
         resPath + "/shader" + "/task12" + "/shader_light.fs");

    float vertices_arr[] = {
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

    std::vector<glm::vec3> pointLightColors {
        glm::vec3(1.0f, 0.6f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0, 0.0),
        glm::vec3(0.2f, 0.2f, 1.0f)
    };

    std::vector<glm::vec3> pointLightPosition {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    std::vector<glm::vec3> cubePositions {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);

    unsigned int lightVBO;
    glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_arr), vertices_arr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);

    float radius = 10.0;

    std::vector<std::string> positionNameBuffer;
    std::vector<std::string> ambientNameBuffer;
    std::vector<std::string> diffuseNameBuffer;
    std::vector<std::string> specularNameBuffer;
    std::vector<std::string> constantNameBuffer;
    std::vector<std::string> linearNameBuffer;
    std::vector<std::string> quadraticNameBuffer;

    std::string lightName = "pointLight";
    std::vector<std::string> attrName {
        ".position",
        ".ambient",
        ".diffuse",
        ".specular",
        ".constant",
        ".linear",
        ".quadratic",
    };

    for (int i = 0; i < pointLightPosition.size(); ++i) {
        positionNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[0]);
        ambientNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[1]);
        diffuseNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[2]);
        specularNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[3]);
        constantNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[4]);
        linearNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[5]);
        quadraticNameBuffer.push_back(lightName + "[" + std::to_string(i) + "]" + attrName[6]);
    }

    Model ourModel(resPath + "/model" + "/backpack" + "/backpack.obj");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float rotatedAngle = 0.0f;
        if (isPause) {
            // if (!startPause) {
            //     lastPauseTime = currentFrame;
            //     startPause = true;
            // }
            pauseTime += deltaTime;
            
        } 

        lightingShader.use();

        const auto &cameraPos = camera.getCameraPos();

        lightingShader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

        lightingShader.setFloat("material.shininess", 32.0f);

        for (int i = 0; i < pointLightPosition.size(); ++i) {
            lightingShader.setVec3(positionNameBuffer[i], pointLightPosition[i]);
            lightingShader.setVec3(ambientNameBuffer[i], pointLightColors[i].x * 0.1f, pointLightColors[i].y * 0.1f, pointLightColors[i].z * 0.1f);
            lightingShader.setVec3(diffuseNameBuffer[i], pointLightColors[i]);
            lightingShader.setVec3(specularNameBuffer[i], 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat(constantNameBuffer[i], 1.0f);
            lightingShader.setFloat(linearNameBuffer[i], 0.09f);
            lightingShader.setFloat(quadraticNameBuffer[i], 0.032f);
        }

        lightingShader.setVec3("dirLight.direction", -0.2, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.1f);
        lightingShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.7f);
        lightingShader.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);

        lightingShader.setVec3("spotLight.position", cameraPos);
        lightingShader.setVec3("spotLight.direction", camera.getCameraFront());
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);

        glm::mat4 view;

        view = camera.getViewMatrix();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        ourModel.draw(lightingShader);
        

        lightShader.use();
        glBindVertexArray(lightVAO);

        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        
        // setMat(lightShader.getUniformLoc("view"), view);
        // setMat(lightShader.getUniformLoc("projection"), projection);

        for (int i = 0; i < pointLightPosition.size(); ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPosition[i]);
            model = glm::scale(model, glm::vec3 {0.2f, 0.2f, 0.2f});
            lightShader.setMat4("model", model);
            lightShader.setVec3("objectColor", pointLightColors[i]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //glUseProgram(shaderProgram);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
