#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "task6.hpp"
#include "FileShader.hpp"

#include <stb_image.h>

#define STR(str) #str
#define STRING(str) STR(str)

namespace task6 {
    float mixValue = 0.2f;

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
    }

    void setMat(GLint pos, glm::mat4 matrix) {
        glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

int main6(int argc, char const *argv[])
{
    using namespace task6;

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

     if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    std::string rootpath(STRING(SOURCE_ROOT)); rootpath = "resoures";

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load((rootpath + "/texture" + "/container.jpg").c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load((rootpath + "/texture" + "/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    FileShader fileShader(
         rootpath + "/shader" + "/shader4.vs",
         rootpath + "/shader" + "/shader4.fs");

    fileShader.use();
    glUniform1i(glGetUniformLocation(fileShader.getId(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(fileShader.getId(), "texture2"), 1);
    auto mixValueLoc = glGetUniformLocation(fileShader.getId(), "mixValue");

    float vertices[] = {
         0.5f, -0.5f,  0.5f, 2.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 2.0f, 2.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 2.0f, 2.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 2.0f,

        -0.5f, -0.5f, -0.5f, 2.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 2.0f, 2.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 2.0f, 2.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 2.0f,

        -0.5f, -0.5f,  0.5f, 2.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 2.0f, 2.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 2.0f, 2.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 2.0f,

         0.5f, -0.5f, -0.5f, 2.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 2.0f, 2.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 2.0f, 2.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 2.0f,

         0.5f,  0.5f,  0.5f, 2.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 2.0f, 2.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 2.0f, 2.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 2.0f,

         0.5f, -0.5f, -0.5f, 2.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 2.0f, 2.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 2.0f, 2.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 2.0f,
    };

    std::vector<glm::vec3> cubePositions {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.3f,  1.0f, -1.5f),
        glm::vec3( 3.3f, -4.0f, -12.0f),
        glm::vec3( 2.8f,  1.4f, -7.5f),
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fileShader.use();
        glBindVertexArray(VAO);

        glUniform1f(mixValueLoc, mixValue);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        setMat(fileShader.getUniformLoc("view"), view);
        setMat(fileShader.getUniformLoc("projection"), projection);

        for (std::size_t i = 0; i < cubePositions.size(); ++i) {
            auto time = glfwGetTime();
            int time_ms = (int) (time * 1000);
            float angle = 0.0f;
            int gap = 5000;
            auto ratio = (time_ms % gap) / (float) gap * 24.0f - 12.0f;
            auto trans = exp(ratio) / (1 + exp(ratio)) * gap + time_ms / gap * gap;
            angle = (i + 1) * 15.0f + trans * 0.01 * -15.0f;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            setMat(fileShader.getUniformLoc("model"), model);
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
