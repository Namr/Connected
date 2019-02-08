#pragma once

#include <qopenglfunctions_4_0_core.h>
#include <qopenglwidget.h>
#include <qfile.h>

#include <string>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include "Camera.h"

#include "tiny_obj_loader.h"
class Model
{
	GLuint loadShader(QOpenGLFunctions_4_0_Core *f, const char *filepath, GLenum type);
	void GLInit(QOpenGLFunctions_4_0_Core *f);

	unsigned int VAO, VBO, EBO;
	GLuint shaderProgram;
	GLint uniTrans, uniView, uniProj, uniColor, uniParent;
	GLint uniIsTexOn, uniTexture, uniLayer, uniMRIView;
	std::vector<GLuint> triangles;
	std::vector<float> vertices;
	std::vector<float> normals;

public:
	Model();
	void loadFromObj(QOpenGLFunctions_4_0_Core *f, std::string path, int hasTextures);
	void addTexture(QOpenGLFunctions_4_0_Core *f, int id);
	void render(QOpenGLFunctions_4_0_Core *f, Camera &camera);
	void render(QOpenGLFunctions_4_0_Core *f, Camera &camera, float r, float g, float b, float a);
	float layer = 0.0f;
	int view = 0;
	glm::mat4 model;
	glm::mat4 parentPosition;
};
