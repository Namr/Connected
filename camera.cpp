#include "camera.h"

Camera::Camera()
{

}
Camera::Camera(int w, int h)
{
    view = glm::lookAt(
        glm::vec3(2.0f, -200.0f, 70.2f), // position
        glm::vec3(0.0f, 0.0f, 0.0f), // camera center
        glm::vec3(0.0f, 0.0f, 1.0f) // up axis
    );
    proj = glm::perspective(glm::radians(45.0f), static_cast<float>(w) / h, 1.0f, 2000.0f);
}

void Camera::Resize(int w, int h)
{
    proj = glm::perspective(glm::radians(45.0f), static_cast<float>(w) / h, 1.0f, 2000.0f);
}
