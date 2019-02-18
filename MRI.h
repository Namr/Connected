#pragma once


#include <qopenglfunctions_3_3_core.h>
#include <qopenglwidget.h>
#include <qfile.h>
#include "NIFTI.h"
#include "camera.h"
#include <glm/glm.hpp>
#include "model.h"

class MRI
{
	GLuint loadShader(QOpenGLFunctions_3_3_Core *f, const char *filepath, GLenum type);
	float map(float s, float a1, float a2, float b1, float b2);
public:
	MRI(QOpenGLFunctions_3_3_Core *f);
	MRI();
	NIFTI data;
	Model plane;
	glm::mat4 coronal = glm::mat4(1.0f);
	glm::mat4 axial = glm::mat4(1.0f);
	float coronalPosition = -90.0f;
	float axialPosition = -80.0f;
	void render(QOpenGLFunctions_3_3_Core *f, Camera &camera);
	~MRI();
};

