#include "CameraGL.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

void CameraGL::updateViewMatrix()
{
    _viewMatrix = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

void CameraGL::updateVectors()
{
    _cameraFront = glm::normalize(glm::vec3 {
        cos(glm::radians(_pitch)) * cos(glm::radians(_yaw)),
        sin(glm::radians(_pitch)),
        cos(glm::radians(_pitch)) * sin(glm::radians(_yaw))});
    
    _cameraRight = glm::normalize(glm::cross(_cameraFront, _worldUp));
    _cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraFront));
}

CameraGL::CameraGL(
        glm::vec3 pos, 
        float yaw, 
        float pitch, 
        glm::vec3 up,
        float speed,
        float sensitivity,
        float fov) 
        : _cameraPos(pos), _yaw(yaw), _pitch(pitch), _worldUp(up), _speed(speed), _sensitivity(sensitivity), _fov(fov) 
    {
        updateVectors();
        updateViewMatrix();
    }

glm::mat4 CameraGL::getViewMatrix() const
{
    return _viewMatrix;
}

glm::mat4 CameraGL::getForcedUpdatedViewMatrix()
{
    updateViewMatrix();
    return _viewMatrix;
}

void CameraGL::processKeyboard(Camera_Movement position, float deltaTime)
{
    const float cameraSpeed = deltaTime * _speed;
    auto tmpFront = _cameraFront;
    if (trueFps) {
        tmpFront.y = 0;
        tmpFront = glm::normalize(tmpFront);
    }

    if (position == Camera_Movement::FORWARD) {
        _cameraPos += tmpFront * cameraSpeed;
    } else if (position == Camera_Movement::BACKWARD) {
        _cameraPos -= tmpFront * cameraSpeed;
    } else if (position == Camera_Movement::LEFT) {
        _cameraPos -= _cameraRight * cameraSpeed;
    } else if (position == Camera_Movement::RIGHT) {
        _cameraPos += _cameraRight * cameraSpeed;
    }
    updateViewMatrix();
}

void CameraGL::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    _yaw += xoffset * _sensitivity;
    _pitch += yoffset * _sensitivity;

    if (constrainPitch) {
        _pitch = std::max(_pitchDownConstrain, std::min(_pitchUpConstrain, _pitch));
    }

    updateVectors();
    updateViewMatrix();
}

void CameraGL::processMouseScroll(float yoffset)
{
    _fov = std::min(_fovMax, std::max(_fovMin, static_cast<float>(_fov - yoffset)));
}

float CameraGL::getFov() const {
    return _fov;
}