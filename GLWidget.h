#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <qopenglwidget.h>
#include <qopenglfunctions_3_2_core.h>
#include <qopenglfunctions.h>
#include <QPainter>

#include <qtimer.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qlabel.h>

#include "camera.h"
#include "mri.h"
#include "brain.h"
#include "colorsettings.h"
#include <ctime>

struct NText
{
    float x;
    float y;
    QString str;
};

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
    void flipView();
    void renderText(glm::mat4 model, Camera cam, glm::vec4 viewport, const QString &str);
    ~GLWidget();

    QPainter painter;

    Brain primaryBrain;
    int primaryShouldReload = 0;
    std::string primaryNodeName;
    std::string primaryEdgeName;

    Brain secondaryBrain;
    int secondaryShouldReload = 0;
    std::string secondaryNodeName;
    std::string secondaryEdgeName;

    QLabel *nodeName;

    int leftKeyDown = 0;
    int rightKeyDown = 0;
    int upKeyDown = 0;
    int downKeyDown = 0;
    int leftMouseDown = 0;

    NColor *colors;
    std::vector<NText> nodeTexts;
    float *nodeSize;
    float *connectionSize;
    float *graphSignalSize;
    float *threshold;
    float *textThreshold;
    int *textSize;
    MTransform *coronal;
    MTransform *axial;
private:
    int WIDTH;
    int HEIGHT;

    GLuint screenFramebuffer = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
    Camera cam;
    Camera top;
    Camera side;
    Camera front;
    time_t lastTime;
    float turnSpeed = 0.8f;
    float yaw = -30.0f;
    float pitch = 90.0f;
    int selectedNode = 0;
};

#endif // GLWIDGET_H
