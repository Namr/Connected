#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#define GLCheckError(id) (id != -1) ? static_cast<unsigned int>(id) : static_cast<unsigned int>(printf("ERROR: GL Data Location Error"));

#include <qopenglfunctions_3_2_core.h>
#include <qmessagebox.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
class screenQuad
{
    GLuint loadShader(QOpenGLFunctions_3_2_Core *f, const char *filepath, GLenum type);

    unsigned int VAO, VBO;
    GLint uniTexture, uniDepth;
    GLuint shaderProgram;
    float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
public:
    screenQuad();
    void init(QOpenGLFunctions_3_2_Core *f);
    void render(QOpenGLFunctions_3_2_Core *f);
};

#endif // SCREENQUAD_H
