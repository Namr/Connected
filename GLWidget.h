#pragma once

#include <qopenglwidget.h>
#include <qopenglfunctions_4_0_core.h>
#include <qtimer.h>
#include <qevent.h>
#include <qmessagebox.h>
#include "Camera.h"
#include "Brain.h"
#include <ctime>

class GLWidget :
	public QOpenGLWidget
{
		Q_OBJECT
public:
	int viewingMode = 1; //modes: 1 = multiview 2 = orbit cam only 3 = comparason mode
	GLWidget(QWidget *parent = 0);
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	~GLWidget();

	Brain primaryBrain;
	Brain secondaryBrain;
	int leftKeyDown = 0;
	int rightKeyDown = 0;
	int upKeyDown = 0;
	int downKeyDown = 0;
private:
	QTimer *frameTimer;

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
protected:
	bool eventFilter(QObject* obj, QEvent* event);
};

