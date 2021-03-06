#ifndef BRAIN_H
#define BRAIN_H

#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <vector>
#include <fstream>
#include <math.h>

#include <qopenglfunctions_3_2_core.h>
#include <qopenglwidget.h>
#include <qmessagebox.h>
#include <qdatetime.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>

#include "model.h"
#include "brainmodel.h"
#include "camera.h"
#include "mri.h"
#include "colorsettings.h"
#include <boost/algorithm/string.hpp>

class GLWidget;

class Brain
{
    Model sphere;
    Model connector;
    std::vector<glm::mat4> nodePositions;
    std::vector<int> nodeColors;
    std::vector<float> nodeSizes;
    std::vector<std::vector<std::vector<float>>> connections;

    NColor rgb2hsv(NColor in);
    NColor hsv2rgb(NColor in);
    float map(float s, float a1, float a2, float b1, float b2);
public:
    Brain(QOpenGLFunctions_3_2_Core *f, std::string nodePath, QStringList connectionPath);
    Brain();
    void reloadBrain(std::string nodePath, QStringList connectionPath);
    void update(QOpenGLFunctions_3_2_Core *f, Camera &camera, float xpos, float ypos, int &selectedNode, int mouseDown);
    void setPosition(glm::vec3 position);
    void updatePosition();
    void loadAppendedNodeData(std::string filepath);

    BrainModel mesh;
    glm::mat4 position;

    GLWidget *screen;
    std::vector<std::string> nodeNames;
    float threshold = 0.5f;
    float textThreshold = 1.0f;

    NColor *colors;
    NColor blue;
    NColor red;
    NColor white;
    NColor black;
    float lerp(float a, float b, float f);
    NColor clerp(NColor a, NColor b, float f);
    float nodeSize = 1.5f;
    bool isScaling = false;
    float connectionSize = 0.3f;
    float graphSignalSize = 20.0f;

    int displayShell = 1;
    int displayMri = -1;
    MRI mri;

    bool hasAppendedData = false;
    bool displayHeatMap = false;
    bool connectionStrengthColor = true;
    bool hasTime = false;
    int numFrames = 0;

    std::vector<std::vector<float>> appendedNodeData;
    float currentFrame = 0.0f;

    int *milisecondsPerFrame;
    quint64 nextFrameTime = 0;
};



#endif // BRAIN_H
