#ifndef BRAIN_H
#define BRAIN_H

#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <vector>
#include <fstream>

#include <qopenglfunctions_3_2_core.h>
#include <qopenglwidget.h>
#include <qmessagebox.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>

#include "model.h"
#include "camera.h"
#include "mri.h"
#include "colorsettings.h"
#include <boost/algorithm/string.hpp>

class Brain
{
    Model sphere;
    Model mesh;
    Model connector;
    std::vector<glm::mat4> nodePositions;
    std::vector<int> nodeColors;
    std::vector<std::vector<float>> connections;
    glm::mat4 position;

    std::vector<float> appendedNodeData;
    int hasAppendedData = 0;
public:
    Brain(QOpenGLFunctions_3_2_Core *f, std::string nodePath, std::string connectionPath);
    Brain();
    void reloadBrain(std::string nodePath, std::string connectionPath);
    void update(QOpenGLFunctions_3_2_Core *f, Camera &camera, float xpos, float ypos, int &selectedNode, int mouseDown);
    void setPosition(glm::vec3 position);

    void loadAppendedNodeData(std::string filepath);
    std::vector<std::string> nodeNames;
    float threshold = 0.5;

    NColor *colors;

    int displayShell = 1;
    int displayMri = -1;
    MRI mri;
};



#endif // BRAIN_H
