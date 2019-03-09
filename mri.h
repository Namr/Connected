#ifndef MRI_H
#define MRI_H

#include <qopenglfunctions_3_2_core.h>
#include <qopenglwidget.h>
#include <qfile.h>
#include "nifti.h"
#include "camera.h"
#include <glm/glm.hpp>
#include "model.h"

struct MTransform
{
    float x, y, z;
    float sx, sy, sz;
};

class MRI
{
    GLuint loadShader(QOpenGLFunctions_3_2_Core *f, const char *filepath, GLenum type);
    float map(float s, float a1, float a2, float b1, float b2);
public:
    MRI(QOpenGLFunctions_3_2_Core *f);
    MRI();
    NIFTI data;
    Model plane;
    MTransform coronalTrans;
    MTransform axialTrans;
    glm::mat4 coronal = glm::mat4(1.0f);
    glm::mat4 axial = glm::mat4(1.0f);
    float coronalPosition = -90.0f;
    float axialPosition = 50;
    void render(QOpenGLFunctions_3_2_Core *f, Camera &camera);
    ~MRI();
};

#endif
