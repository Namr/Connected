#pragma once

#include <qopenglwidget.h>
#include <qopenglfunctions_4_0_core.h>
#include "Camera.h"
#include "Brain.h"
#include <ctime>

class GLWidget :
	public QOpenGLWidget
{
		Q_OBJECT
public:
	GLWidget(QWidget *parent = 0);
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	~GLWidget();
private:
	Brain bigboi;
	Brain smolboi;
	GLuint screenFramebuffer = 0;
	GLuint renderedTexture;
	GLuint depthrenderbuffer;
	Camera cam;
	Camera top;
	Camera side;
	Camera front;
	time_t lastTime;
	float speed = 0.1f;
	float turnSpeed = 0.4f;
	float yaw = -30.0f;
	float pitch = 90.0f;
	int selectedNode = -1;
	int viewingMode = 1; //modes: 1 = multiview 2 = orbit cam only 3 = comparason mode
};

