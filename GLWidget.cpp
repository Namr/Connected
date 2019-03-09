#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent):
    QOpenGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

void GLWidget::initializeGL()
{
    WIDTH = this->width();
    HEIGHT = this->height();

    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    if (!f) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "This machine does not seem to support Modern OpenGL (>=3.2)");
        messageBox.setFixedSize(500, 200);
    }

    f->initializeOpenGLFunctions();

    f->glEnable(GL_BLEND);
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    f->glGenFramebuffers(1, &screenFramebuffer);
    f->glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);

    // The texture we're going to render to
    f->glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    f->glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    f->glGenRenderbuffers(1, &depthrenderbuffer);
    f->glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    // Set "renderedTexture" as our colour attachement #0
    f->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    f->glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


    //set background color
    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    primaryBrain = Brain(f, "C:/Users/vrdem/Documents/GitHub/Connected/assets/Node_AAL116.node",
        "C:/Users/vrdem/Documents/GitHub/Connected/assets/connect.edge");
    secondaryBrain = Brain(f, "C:/Users/vrdem/Documents/GitHub/Connected/assets/Node_AAL116.node",
        "C:/Users/vrdem/Documents/GitHub/Connected/assets/connect2.edge");

    primaryBrain.screen = this;
    secondaryBrain.screen = this;

    cam = Camera(WIDTH, HEIGHT);;
    top = Camera(WIDTH / 2, HEIGHT / 2);
    side = Camera(WIDTH / 2, HEIGHT / 2);
    front = Camera(WIDTH / 2, HEIGHT / 2);

    top.position = glm::vec3(0.57f, 6.21f, 228.85f);
    top.altPosition = glm::vec3(1.57f, -40.93f, -228.85f);
    top.view = glm::lookAt(
        top.position, // position
        glm::vec3(1.52f, -33.28f, -80.23f), // camera center
        glm::vec3(0.0f, 0.0f, 1.0f) // up axis
    );
    front.position = glm::vec3(0.41f, 156.71f, 19.51f);
    front.altPosition = glm::vec3(0.41f, -220.92f, 19.51f);
    front.view = glm::lookAt(
        front.position, // position
        glm::vec3(1.52f, -33.28f, 6.23f), // camera center
        glm::vec3(0.0f, 0.0f, -1.0f) // up axis
    );
    side.position = glm::vec3(183.95f, -36.63f, -29.99f);
    side.altPosition = glm::vec3(-183.733f, -36.63f, -29.99f);
    side.view = glm::lookAt(
        side.position, // position
        glm::vec3(1.52f, -33.28f, 6.23f), // camera center
        glm::vec3(0.0f, 0.0f, -1.0f) // up axis
    );

    cam.position = front.position;
}

void GLWidget::resizeGL(int w, int h)
{

    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    WIDTH = w;
    HEIGHT = h;

    //bind to our framebuffer and change the texture size
    f->glBindTexture(GL_TEXTURE_2D, renderedTexture);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    //do the same with the depth buffer
    f->glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

    cam.Resize(WIDTH, HEIGHT);;
    top.Resize(WIDTH / 2, HEIGHT / 2);
    side.Resize(WIDTH / 2, HEIGHT / 2);
    front.Resize(WIDTH / 2, HEIGHT / 2);
}

void GLWidget::paintGL()
{
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

    float xpos = this->mapFromGlobal(QCursor::pos()).x();
    float ypos = this->mapFromGlobal(QCursor::pos()).y();

    float deltaTime = 0.01f;

    if (nodeName != nullptr)
    {
        nodeName->setText(primaryBrain.nodeNames[selectedNode].c_str());
    }

    f->glEnable(GL_BLEND);
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //update varibles
    primaryBrain.colors = colors;
    secondaryBrain.colors = colors;

    primaryBrain.threshold = *threshold;
    secondaryBrain.threshold = *threshold;

    primaryBrain.textThreshold = *textThreshold;
    secondaryBrain.textThreshold = *textThreshold;

    primaryBrain.nodeSize = *nodeSize;
    secondaryBrain.nodeSize = *nodeSize;

    primaryBrain.connectionSize = *connectionSize;
    secondaryBrain.connectionSize = *connectionSize;

    primaryBrain.graphSignalSize = *graphSignalSize;
    secondaryBrain.graphSignalSize = *graphSignalSize;

    primaryBrain.mri.axialTrans = *axial;
    primaryBrain.mri.coronalTrans = *coronal;
    secondaryBrain.mri.axialTrans = *axial;
    secondaryBrain.mri.coronalTrans = *coronal;

    if (primaryShouldReload == 1)
    {
        primaryBrain.reloadBrain(primaryNodeName, primaryEdgeName);
        primaryShouldReload = 0;
    }
    else if (secondaryShouldReload == 1)
    {
        secondaryBrain.reloadBrain(secondaryNodeName, secondaryEdgeName);
        secondaryShouldReload = 0;
    }


    if (upKeyDown == 1)
    {
        pitch -= turnSpeed * deltaTime;
    }
    if (downKeyDown == 1)
    {
        pitch += turnSpeed * deltaTime;
    }
    if (rightKeyDown == 1)
    {
        yaw -= turnSpeed * deltaTime;
    }
    if (leftKeyDown == 1)
    {
        yaw += turnSpeed * deltaTime;
    }

    cam.position.x = 1.52f + (cos(yaw)  * sin(pitch) * 220);
    cam.position.y = -33.28f + (sin(yaw) * sin(pitch) * 220);
    cam.position.z = 6.23f * (cos(pitch) * 50);

    if (viewingMode == 3)
        cam.proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH / 2) / HEIGHT, 1.0f, 2000.0f);
    else
        cam.proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 1.0f, 2000.0f);

    cam.view = glm::lookAt(
        cam.position,             // position
        glm::vec3(1.52f, -33.28f, 6.23f), // camera center
        glm::vec3(0.0f, 0.0f, -1.0f)                    // up axis
    );

    f->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFramebuffer);
    f->glClearColor(colors[6].R / 255.0f, colors[6].G / 255.0f, colors[6].B / 255.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_FRAMEBUFFER_SRGB);

    // Render to our framebuffer
    f->glBindFramebuffer(GL_FRAMEBUFFER, screenFramebuffer);

    if (viewingMode == 1)
    {
        f->glViewport(0, 0, WIDTH / 2, HEIGHT / 2);
        primaryBrain.update(f, front, xpos, ypos, selectedNode, leftMouseDown);

        f->glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT / 2);
        primaryBrain.update(f, top, xpos, ypos, selectedNode, leftMouseDown);

        f->glViewport(0, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
        primaryBrain.update(f, side, xpos, ypos, selectedNode, leftMouseDown);

        f->glViewport(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
        primaryBrain.update(f, cam, xpos, ypos, selectedNode, leftMouseDown);
    }
    else if (viewingMode == 2)
    {
        f->glViewport(0, 0, WIDTH, HEIGHT);
        primaryBrain.update(f, cam, xpos, ypos, selectedNode, leftMouseDown);
    }
    else if (viewingMode == 3)
    {
        f->glViewport(0, 0, WIDTH / 2, HEIGHT);
        primaryBrain.update(f, cam, xpos, ypos, selectedNode, leftMouseDown);

        f->glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
        secondaryBrain.update(f, cam, xpos, ypos, selectedNode, leftMouseDown);
    }
    f->glBindFramebuffer(GL_READ_FRAMEBUFFER, screenFramebuffer);
    f->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dispFramebuffer);
    f->glBlitFramebuffer(0, HEIGHT, WIDTH, 0,
        0, 0, WIDTH, HEIGHT,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    f->glBindFramebuffer(GL_READ_FRAMEBUFFER, dispFramebuffer);

    //do anything related to QPainter now
    // Render text
    painter.begin(this);
    painter.setPen(QColor(colors[8].R, colors[8].G, colors[8].B));
    painter.setFont(QFont("Times", 10, QFont::Bold));
    for(NText text: nodeTexts)
    {
        painter.drawText(text.x, text.y, text.str);
    }
    nodeTexts.clear();
    painter.end();
}

void GLWidget::flipView()
{
    float xpos = this->mapFromGlobal(QCursor::pos()).x();
    float ypos = this->mapFromGlobal(QCursor::pos()).y();
    if (xpos < (WIDTH / 2))
    {
        if (ypos < (HEIGHT / 2))
        {
            front.viewMode *= -1;
            if (front.viewMode == 1)
            {
                front.view = glm::lookAt(
                    front.position, // position
                    glm::vec3(1.52f, -33.28f, 6.23f), // camera center
                    glm::vec3(0.0f, 0.0f, -1.0f) // up axis
                );
            }
            else
            {
                front.view = glm::lookAt(
                    front.altPosition, // position
                    glm::vec3(1.52f, -33.28f, 6.23f), // camera center
                    glm::vec3(0.0f, 0.0f, -1.0f) // up axis
                );
            }
        }
        else
        {
            side.viewMode *= -1;
            if (side.viewMode == 1)
            {
                side.view = glm::lookAt(
                    side.position, // position
                    glm::vec3(1.52f, -33.28f, 6.23f), // camera center
                    glm::vec3(0.0f, 0.0f, -1.0f) // up axis
                );
            }
            else
            {
                side.view = glm::lookAt(
                    side.altPosition, // position
                    glm::vec3(1.52f, -33.28f, 6.23f), // camera center
                    glm::vec3(0.0f, 0.0f, -1.0f) // up axis
                );
            }
        }
    }
    else
    {
        if (ypos < (HEIGHT / 2))
        {
            top.viewMode *= -1;
            if (top.viewMode == 1)
            {
                top.view = glm::lookAt(
                    top.position, // position
                    glm::vec3(1.52f, -33.28f, -80.23f), // camera center
                    glm::vec3(0.0f, 0.0f, 1.0f) // up axis
                );
            }
            else
            {
                top.view = glm::lookAt(
                    top.altPosition, // position
                    glm::vec3(1.52f, -33.28f, -80.23f), // camera center
                    glm::vec3(0.0f, 0.0f, 1.0f) // up axis
                );
            }
        }
    }
}

void GLWidget::renderText(glm::mat4 model, Camera cam, glm::vec4 viewport, const QString &str)
{
    // Identify x and y locations to render text within widget
    glm::vec3 textPos = glm::vec3(0,0,0);
    textPos = glm::project(glm::vec3(model[3]), cam.view, cam.proj, viewport);

    NText text;
    text.x = textPos.x;
    text.y = textPos.y;
    text.str = str;
    nodeTexts.push_back(text);
}

GLWidget::~GLWidget()
{
}
