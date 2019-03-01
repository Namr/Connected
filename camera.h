#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 position;
    glm::vec3 altPosition;
    int viewMode = 1; // 1 default, -1 alt
    Camera();
    Camera(int w, int h);
    void Resize(int w, int h);
};



#endif // CAMERA_H
