#ifndef BRAINMODEL_H
#define BRAINMODEL_H

#define GLCheckError(id) (id != -1) ? static_cast<unsigned int>(id) : static_cast<unsigned int>(printf("ERROR: GL Data Location Error"));

#include <qopenglfunctions_3_2_core.h>
#include <qopenglwidget.h>
#include <qfile.h>
#include <qmessagebox.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include "camera.h"
#include "tiny_obj_loader.h"

class Brain;

class BrainModel
{
    GLuint loadShader(QOpenGLFunctions_3_2_Core *f, const char *filepath, GLenum type);
    void GLInit(QOpenGLFunctions_3_2_Core *f);

    unsigned int VAO, VBO, EBO;
    GLuint shaderProgram;
    GLint uniTrans, uniView, uniProj, uniColor, uniParent;
    GLint uniIsTexOn, uniTexture, uniLayer, uniMRIView, uniIsColor, uniNodeColors;
    std::vector<GLuint> triangles;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::map<int, int> idLookUp;
public:
    BrainModel();
    void loadFromObj(QOpenGLFunctions_3_2_Core *f, std::string path, int hasTextures);
    void loadFromNV(QOpenGLFunctions_3_2_Core *f, std::string path);
    void loadFromVTK(QOpenGLFunctions_3_2_Core *f, std::string VTKpath, std::string IDAtlas, std::string lookupTable);
    void addTexture(QOpenGLFunctions_3_2_Core *f, int id);
    void render(QOpenGLFunctions_3_2_Core *f, Camera &camera);
    void render(QOpenGLFunctions_3_2_Core *f, Camera &camera, float r, float g, float b, float a);
    float layer = 0.0f;
    int view = 0;
    glm::mat4 model;
    glm::mat4 parentPosition;
    bool isColored = false;
    Brain *brain;
};

#endif // BRAINMODEL_H
