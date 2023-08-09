#ifndef CameraGL_HPP
#define CameraGL_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class CameraGL
{
private:
    bool trueFps = true;

    float _pitchUpConstrain = 89.0f;
    float _pitchDownConstrain = -_pitchUpConstrain;

    float _fovMax = 80.0f;
    float _fovMin = 1.0f;

    float _pitch;
    float _yaw;
    float _fov;
    float _speed;
    float _sensitivity;

    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraRight;
    glm::vec3 _cameraUp;
    glm::vec3 _worldUp;

    glm::mat4 _viewMatrix;

    void updateViewMatrix();
    void updateVectors();
public:
    constexpr static float DEFAULT_YAW = -90.0f;
    constexpr static float DEFAULT_PITCH = 0.0f;
    constexpr static float DEFAULT_FOV = 45.0f;
    constexpr static float DEFAULT_SPEED = 5.0f;
    constexpr static float DEFAULT_SENSITIVITY = 0.03f;

    CameraGL(
        glm::vec3 pos = {0.0f, 0.0f, 0.0f}, 
        float yaw = DEFAULT_YAW, 
        float pitch = DEFAULT_PITCH, 
        glm::vec3 up = {0.0f, 1.0f, 0.0f},
        float speed = DEFAULT_SPEED,
        float sensitivity = DEFAULT_SENSITIVITY,
        float fov = DEFAULT_FOV);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getForcedUpdatedViewMatrix();

    void processKeyboard(Camera_Movement position, float deltaTime);

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    void processMouseScroll(float yoffset);

    float getFov() const;

    glm::vec3 getCameraPos() const;

    glm::vec3 getCameraFront() const;
};

#endif