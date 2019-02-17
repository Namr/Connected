#include "Brain.h"

Brain::Brain()
{

}

Brain::Brain(QOpenGLFunctions_4_0_Core *f, std::string nodePath, std::string connectionPath)
{
	std::ifstream nodeFile;
	nodeFile.open(nodePath);
	std::string line;

	//iterate over each line in the node file if it is found, store it in line, and operate on it
	if (nodeFile.is_open())
	{
		int nodeNum = 0;
		while (getline(nodeFile, line))
		{
			//split line by spaces so the xyz components are isolated, then create a vec3 to store that position and store it in a list
			std::vector<std::string> tokenized;
			boost::split(tokenized, line, [](char c) { return c == ' ' || c == '	'; });

			float x = stof(tokenized[0]);
			float y = stof(tokenized[1]);
			float z = stof(tokenized[2]);
			int colorID = std::stoi(tokenized[3]) - 1;  //subtract one so that way instead of going from 1-6 its 0-5 and fits array notations
			std::string name = tokenized[5];

			glm::mat4 pos = glm::mat4(1.0f);
			pos = glm::translate(pos, glm::vec3(x, y, z));
			nodePositions.push_back(pos);
			nodeNames.push_back(name);
			nodeColors.push_back(colorID);
		}
		nodeFile.close();
	}
	else
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Node File not found! Check your paths");
		messageBox.setFixedSize(500, 200);
	}

	//load in connection data
	std::ifstream connectionFile;
	connectionFile.open(connectionPath);

	//iterate over each line in the connections file (which is a connection) organizes it into a list of lists
	//it is a list of a list of all the nodes connections
	//i.e connections[1] is a list of all the connections to node 1 and connections[1][2] is the strength of the connection between nodes 1 and 2
	if (connectionFile.is_open())
	{
		int nodeNum = 0;
		for (int n = 0; n < nodePositions.size(); n++)
		{
			std::vector<float> nodesConnections;
			for (int c = 0; c < nodePositions.size(); c++)
			{
				getline(connectionFile, line);
				nodesConnections.push_back(stof(line));
			}
			connections.push_back(nodesConnections);
		}
		nodeFile.close();
	}
	else
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Edge File not found! Check your paths");
		messageBox.setFixedSize(500, 200);
	}

	position = glm::mat4(1.0f);

	mri = MRI(f);
	sphere = Model();
	mesh = Model();
	connector = Model();
	sphere.loadFromObj(f, "assets/sphere.obj", 0);
	mesh.loadFromObj(f, "assets/brain.obj", 0);
	connector.loadFromObj(f, "assets/connector.obj", 0);
}

void Brain::reloadBrain(std::string nodePath, std::string connectionPath)
{

	nodePositions.clear();
	nodeNames.clear();
	nodeColors.clear();
	connections.clear();

	std::ifstream nodeFile;
	nodeFile.open(nodePath);
	std::string line;

	//iterate over each line in the node file if it is found, store it in line, and operate on it
	if (nodeFile.is_open())
	{
		int nodeNum = 0;
		while (getline(nodeFile, line))
		{
			//split line by spaces so the xyz components are isolated, then create a vec3 to store that position and store it in a list
			std::vector<std::string> tokenized;
			boost::split(tokenized, line, [](char c) { return c == ' ' || c == '	'; });

			float x = stof(tokenized[0]);
			float y = stof(tokenized[1]);
			float z = stof(tokenized[2]);
			int colorID = std::stoi(tokenized[3]) - 1;  //subtract one so that way instead of going from 1-6 its 0-5 and fits array notations
			std::string name = tokenized[5];

			glm::mat4 pos = glm::mat4(1.0f);
			pos = glm::translate(pos, glm::vec3(x, y, z));
			nodePositions.push_back(pos);
			nodeNames.push_back(name);
			nodeColors.push_back(colorID);
		}
		nodeFile.close();
	}
	else
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Node File not found! Check your paths");
		messageBox.setFixedSize(500, 200);
	}

	//load in connection data
	std::ifstream connectionFile;
	connectionFile.open(connectionPath);

	//iterate over each line in the connections file (which is a connection) organizes it into a list of lists
	//it is a list of a list of all the nodes connections
	//i.e connections[1] is a list of all the connections to node 1 and connections[1][2] is the strength of the connection between nodes 1 and 2
	if (connectionFile.is_open())
	{
		int nodeNum = 0;
		for (int n = 0; n < nodePositions.size(); n++)
		{
			std::vector<float> nodesConnections;
			for (int c = 0; c < nodePositions.size(); c++)
			{
				getline(connectionFile, line);
				nodesConnections.push_back(stof(line));
			}
			connections.push_back(nodesConnections);
		}
		nodeFile.close();
	}
	else
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Edge File not found! Check your paths");
		messageBox.setFixedSize(500, 200);
	}
}

void Brain::setPosition(glm::vec3 pos)
{
	position = glm::mat4(1.0f);
	position = glm::translate(position, pos);

	sphere.parentPosition = position;
	mesh.parentPosition = position;
	connector.parentPosition = position;
	//mri.parentPosition = position;
}

void Brain::update(QOpenGLFunctions_4_0_Core *f, Camera &camera, float xpos, float ypos, int &selectedNode, int mouseDown)
{
	int node = 0;
	for (glm::mat4 pos : nodePositions)
	{
		//move sphere to position and then render it
		sphere.model = pos;
		sphere.model = glm::scale(sphere.model, glm::vec3(1.5f, 1.5f, 1.5f));

		GLint viewportraw[4];
		glGetIntegerv(GL_VIEWPORT, viewportraw);

		glm::vec4 viewport = glm::vec4(viewportraw[0], viewportraw[1], viewportraw[2], viewportraw[3]);
		glm::vec3 v0 = glm::unProject(glm::vec3(xpos, ypos, 0.0f), camera.view, camera.proj, viewport);
		glm::vec3 v1 = glm::unProject(glm::vec3(xpos, ypos, 1.0f), camera.view, camera.proj, viewport);
		glm::vec3 dir = glm::normalize((v1 - v0));

		glm::vec3 hitPos;
		glm::vec3 hitNorm;
		bool hit;
		if (camera.viewMode == 1)
			hit = glm::intersectRaySphere(camera.position, dir, glm::vec3(sphere.model[3]), 2.25f, hitPos, hitNorm);
		else
			hit = glm::intersectRaySphere(camera.altPosition, dir, glm::vec3(sphere.model[3]), 2.25f, hitPos, hitNorm);

		//for each node that this node is connected to, draw a connection
		int connectedNode = 0;
		for (float connection : connections[node])
		{
			if (connection > 0.5 && (mouseDown == 0 || hit || selectedNode == node || selectedNode == connectedNode))
			{
				//move connector to the spheres location, and then aim it at the connected node
				connector.model = glm::mat4(1);
				glm::mat4 look = glm::lookAt(
					glm::vec3(sphere.model[3]),                 // position
					glm::vec3(nodePositions[connectedNode][3]), // looking at
					glm::vec3(0.0f, 0.0f, 1.0f)                 // up axis
				);
				connector.model *= glm::inverse(look); //apply the lookat trasnformation

				//now scale it so it actually reaches that node
				float dist = glm::distance(glm::vec3(sphere.model[3]), glm::vec3(nodePositions[connectedNode][3]));
				connector.model = glm::scale(connector.model, glm::vec3(connection / 3, connection / 3, dist * 0.3));

				//this line ensures the scale occurs from the BASE of the model
				connector.model *= glm::mat4(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, -1, 1);
				connector.render(f, camera, connection, 0.0f, 1.0 - connection, 0.8f);
			}
			connectedNode++;
		}
		if (hit)
		{
			sphere.render(f, camera, 1, 0.0, 0, 1);
			selectedNode = node;
			//glfwSetWindowTitle(window, nodeNames[node].c_str());
		}
		else
			sphere.render(f, camera, colorTables[nodeColors[node]][0], colorTables[nodeColors[node]][1], colorTables[nodeColors[node]][2], 1);

		node++;
	}

	
	if (displayMri == 1)
		mri.render(f, camera);
	
	if (displayShell == 1)
	{
		f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh.render(f, camera, 0.9f, 0.9f, 0.9f, 0.3f);
		f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
