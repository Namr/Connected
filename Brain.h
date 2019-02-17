#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <vector>
#include <fstream>

#include <qopenglfunctions_4_0_core.h>
#include <qopenglwidget.h>
#include <qmessagebox.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>

#include "Model.h"
#include "Camera.h"
#include "MRI.h"
#include <boost/algorithm/string.hpp>

class Brain
{
	Model sphere;
	Model mesh;
	Model connector;
	float colorTables[6][3] = {
		{0.0f, 0.0f, 1.0f}, //blue
		{0.0f, 1.0f, 0.0f}, //green
		{1.0f, 1.0f, 0.0f}, //yellow
		{1.0f, 0.0f, 0.3f}, //pink
		{0.0f, 1.0f, 1.0f}, //teal
		{1.0f, 0.5f, 0.0f} //orange
	};
	std::vector<glm::mat4> nodePositions;
	std::vector<int> nodeColors;
	std::vector<std::vector<float>> connections;
	glm::mat4 position;
public:
	Brain(QOpenGLFunctions_4_0_Core *f, std::string nodePath, std::string connectionPath);
	Brain();
	void reloadBrain(std::string nodePath, std::string connectionPath);
	void update(QOpenGLFunctions_4_0_Core *f, Camera &camera, float xpos, float ypos, int &selectedNode, int mouseDown);
	void setPosition(glm::vec3 position);

	std::vector<std::string> nodeNames;
	float threshold = 0.5;

	int displayShell = 1;
	int displayMri = -1;
	MRI mri;
};


