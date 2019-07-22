#include "brain.h"
#include "GLWidget.h"

Brain::Brain()
{

}

Brain::Brain(QOpenGLFunctions_3_2_Core *f, std::string nodePath, QStringList connectionPath)
{

    reloadBrain(nodePath, connectionPath);

    position = glm::mat4(1.0f);

    mri = MRI(f);
    sphere = Model();
    mesh = Model();
    connector = Model();
    sphere.loadFromObj(f, "assets/sphere.obj", 0);
    mesh.loadFromNV(f, "assets/BrainMesh_ICBM152_smoothed.nv");
    connector.loadFromObj(f, "assets/connector.obj", 0);
}

void Brain::reloadBrain(std::string nodePath, QStringList connectionPaths)
{
    nodePositions.clear();
    nodeNames.clear();
    nodeColors.clear();
    nodeSizes.clear();
    connections.clear();
    currentFrame = 0;

    std::ifstream nodeFile;
    nodeFile.open(nodePath);
    std::string line;

    //make sure numFrames is at the local minimum number it can be
    numFrames = (numFrames > connectionPaths.size() || !hasTime) ? connectionPaths.size() : numFrames;
    hasTime = numFrames > 1 ? true : false;

    //iterate over each line in the node file if it is found, store it in line, and operate on it
    if (nodeFile.is_open())
    {
        int nodeNum = 0;
        while (getline(nodeFile, line))
        {
            if (line.find('#') == std::string::npos) //ignore comments
            {
                //split line by spaces so the xyz components are isolated, then create a vec3 to store that position and store it in a list
                std::vector<std::string> tokenized;
                boost::split(tokenized, line, [](char c) { return c == ' ' || c == '	'; });

                float x = stof(tokenized[0]);
                float y = stof(tokenized[1]);
                float z = stof(tokenized[2]);
                int colorID = std::stoi(tokenized[3]) - 1;  //subtract one so that way instead of going from 1-6 its 0-5 and fits array notations
                float size = std::stof(tokenized[4]);
                std::string name = tokenized[5];

                glm::mat4 pos = glm::mat4(1.0f);
                pos = glm::translate(pos, glm::vec3(x, y, z));
                nodePositions.push_back(pos);
                nodeNames.push_back(name);
                nodeColors.push_back(colorID);
                nodeSizes.push_back(size);
            }
        }
        nodeFile.close();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Node File not found! Check your paths");
        messageBox.setFixedSize(500, 200);
    }

    //iterate over all selected connection files, we pop the front one every loop, so go until its empty
    while(!connectionPaths.isEmpty())
    {
        //load in connection data
        std::ifstream connectionFile;
        connectionFile.open(connectionPaths.front().toStdString());
        connectionPaths.pop_front();

        //check if edge file is single data points per line, or whole node data per line
        if (connectionFile.is_open())
        {
            //get a sample line (thats not a comment) and see how many data points are on it, and read accordingly
            getline(connectionFile, line);
            while (line.find('#') != std::string::npos)
            {
                getline(connectionFile, line);
            }

            std::vector<std::string> tokenTest;
            boost::split(tokenTest, line, [](char c) { return c == ' ' || c == '	';});
            connectionFile.clear();
            connectionFile.seekg(0, std::ios::beg);

            //stores this time stamps connections
            std::vector<std::vector<float>> localConnections;

            //iterate over each line in the connections file (which is a connection) organizes it into a list of lists
            //it is a list of a list of all the nodes connections
            //i.e connections[1] is a list of all the connections to node 1 and connections[1][2] is the strength of the connection between nodes 1 and 2
            if (tokenTest.size() > 1)
            {
                int nodeNum = 0;
                for (int n = 0; n < nodePositions.size(); n++)
                {
                    std::vector<float> nodesConnections;
                    //get each line of the file, if its in a matrix format, split it each data point stands on its own
                    getline(connectionFile, line);
                    std::vector<std::string> tokenized;
                    std::vector<float> tokenizedNums;
                    boost::split(tokenized, line, [](char c) { return c == ' ' || c == '	'; });
                    //turn the string data into float data
                    for (std::string str : tokenized)
                    {
                        if (str.find('#') == std::string::npos && str.find_first_not_of("-0123456789.e") == std::string::npos && str.find_first_of("-0123456789.e") != std::string::npos) //ignore comments
                            tokenizedNums.push_back(std::stof(str));
                    }

                    //concatenate that line of data into the final datastructure
                    nodesConnections.insert(nodesConnections.end(), tokenizedNums.begin(), tokenizedNums.end());
                    localConnections.push_back(nodesConnections);
                }
                nodeFile.close();
            }
            else
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
                    localConnections.push_back(nodesConnections);
                }
                nodeFile.close();
            }
            connections.push_back(localConnections);
        }
        else
        {
            QMessageBox messageBox;
            messageBox.critical(0, "Error", "Edge File not found! Check your paths");
            messageBox.setFixedSize(500, 200);
        }
    }

    int mostConnected = -1;
    for(int x = 0; x < connections[0].size(); x++)
    {
        int numConnected = 0;
        for(int y = 0; y < connections[0][x].size(); y++)
        {
            if(connections[0][x][y] > 0.0f)
            {
                numConnected++;
            }
        }
        if(numConnected > mostConnected)
            mostConnected = numConnected;
    }

    std::cout << mostConnected << std::endl;
    if(mostConnected == 2)
    {
        //load in connection data
        std::ifstream connectionFile;
        connectionFile.open("assets/paths.txt");
        if (connectionFile.is_open())
        {
            while (getline(connectionFile, line))
            {
                std::vector<int> path;
                std::vector<std::string> tokens;
                boost::split(tokens, line, [](char c) { return c == ' ';});
                for(std::string token : tokens)
                {
                    if (token.find('#') == std::string::npos && token.find_first_not_of("-0123456789.e") == std::string::npos && token.find_first_of("-0123456789.e") != std::string::npos)
                        path.push_back(std::stoi(token));
                }
                paths.push_back(path);
            }
            connectionFile.close();
        }
        else
        {
            std::cout << "missing paths, contact ymir fritz" << std::endl;
        }
    }
    for ( const auto &row : paths)
    {
       for ( const auto &s : row ) std::cout << s << ' ';
       std::cout << std::endl;
    }
}

void Brain::loadAppendedNodeData(std::string filepath)
{
    appendedNodeData.clear();
    currentFrame = 0;

    std::ifstream nodeFile;
    nodeFile.open(filepath);
    std::string line;

    //iterate over each line in the graph signal file if it is found, store it in line, and operate on it
    if (nodeFile.is_open())
    {
        while (getline(nodeFile, line))
        {
            std::vector<std::string> tokenized;
            std::vector<float> tokenizedNums;
            boost::split(tokenized, line, [](char c) { return c == ' ' || c == '	' || c == '\r'; });
            //turn the string data into float data
            for (std::string str : tokenized)
            {
                //ignore comments and ensure its only numbers
                if (str.find('#') == std::string::npos && str.find_first_not_of("-0123456789.") == std::string::npos && str.find_first_of("-0123456789.") != std::string::npos)
                    tokenizedNums.push_back(std::stof(str));
            }
            //make sure numFrames is at the local minimum number it can be
            numFrames = (numFrames > tokenizedNums.size() || !hasTime) ?  tokenizedNums.size() : numFrames;
            appendedNodeData.push_back(tokenizedNums);
        }
        nodeFile.close();

        //normalize data, first find min and max values
        float largestValue = -100000;
        float smallestValue = 100000;
        for(unsigned int i = 0; i < appendedNodeData.size(); i++)
        {
            for(unsigned int w = 0; w < appendedNodeData[0].size(); w++)
            {
                if(appendedNodeData[i][w] > largestValue)
                    largestValue = appendedNodeData[i][w];
                else if(appendedNodeData[i][w] < smallestValue)
                    smallestValue = appendedNodeData[i][w];
            }
        }
        //now interpolate linearly between these two min and max values
        for(unsigned int i = 0; i < appendedNodeData.size(); i++)
        {
            for(unsigned int w = 0; w < appendedNodeData[0].size(); w++)
            {
                appendedNodeData[i][w] = map(appendedNodeData[i][w], smallestValue, largestValue, -1.0, 1.0);
            }
        }

        hasAppendedData = true;
        hasTime = numFrames > 1 ? true : false;
        nextFrameTime = QDateTime::currentMSecsSinceEpoch() + (*milisecondsPerFrame / 10);
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Graph Signal Data File not found! Check your paths");
        messageBox.setFixedSize(500, 200);
    }
}

void Brain::setPosition(glm::vec3 pos)
{
    position = glm::mat4(1.0f);
    position = glm::translate(position, pos);

    updatePosition();
}

void Brain::updatePosition()
{
    sphere.parentPosition = position;
    mesh.parentPosition = position;
    connector.parentPosition = position;
}

void Brain::update(QOpenGLFunctions_3_2_Core *f, Camera &camera, float xpos, float ypos, int &selectedNode, int mouseDown)
{
    GLint viewportraw[4];
    f->glGetIntegerv(GL_VIEWPORT, viewportraw);

    glm::vec4 viewport = glm::vec4(viewportraw[0], viewportraw[1], viewportraw[2], viewportraw[3]);
    int node = 0;
    for (glm::mat4 pos : nodePositions)
    {
        //move sphere to position and then render it
        sphere.model = pos;
        if(isScaling)
            sphere.model = glm::scale(sphere.model, glm::vec3(nodeSize * nodeSizes[node], nodeSize * nodeSizes[node], nodeSize * nodeSizes[node]));
        else
            sphere.model = glm::scale(sphere.model, glm::vec3(nodeSize, nodeSize, nodeSize));

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
        bool shouldRenderText = false;

        int brainFrame = currentFrame > (connections.size() - 1) ? (connections.size() - 1) : currentFrame;
        if(paths.size() > 0)
        {
        }
        else{
        for (float connection : connections[brainFrame][node])
        {
            //if this statisfies rendering text, change the flag
            if(connection > textThreshold && connectedNode != node)
            {
                shouldRenderText = true;
            }
            //filter out connections below threshold or filter out non isolated nodes if the mouse is down
            if (connection > threshold && (mouseDown == 0 || hit || selectedNode == node || selectedNode == connectedNode))
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
                connector.model = glm::scale(connector.model, glm::vec3(connection * connectionSize, connection * connectionSize, dist * 0.5));
                //this line ensures the scale occurs from the BASE of the model
                connector.model *= glm::mat4(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, -1, 1);

                connector.render(f, camera, connection, 0.0f, 1.0 - connection, 0.8f);
            }
            connectedNode++;
        }}
        if (hasAppendedData && !displayHeatMap) //if we have appended data, render it
        {
            //linear interpolation between the current singal size and the next frames size
            float signalSize = lerp(appendedNodeData[node][floor(currentFrame)], appendedNodeData[node][ceil(currentFrame)], currentFrame - floor(currentFrame)) ;

            connector.model = glm::mat4(1);
            connector.model = glm::translate(connector.model, glm::vec3(sphere.model[3]));
            glm::quat rot = glm::quat(glm::vec3(1.5708f, 0.0f, 1.5708f));
            connector.model = connector.model * glm::mat4_cast(rot);
            connector.model = glm::scale(connector.model, glm::vec3(0.7f, -signalSize * graphSignalSize, 0.7));

            //this line ensures the scale occurs from the BASE of the model
            connector.model *= glm::mat4(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, -1, 1);

            //color change depending on negative/positive values
            if(signalSize >= 0)
                connector.render(f, camera, 1.0f - abs(signalSize * 2), 1.0f - abs(signalSize * 2), 1.0f, 1.0f);
            else
                connector.render(f, camera, 1.0f, 1.0f - abs(signalSize * 2), 1.0f - abs(signalSize * 2), 1.0f);
        }
        if (hit)
        {
            sphere.render(f, camera, 1, 0.0, 0, 1);
            selectedNode = node;
        }
        else
        {
            if(hasAppendedData && displayHeatMap)
            {
                //linear interpolation between the current singal size and the next frames size
                float signalSize = lerp(appendedNodeData[node][floor(currentFrame)], appendedNodeData[node][ceil(currentFrame)], currentFrame - floor(currentFrame)) ;
                if(signalSize >= 0)
                    sphere.render(f, camera, 1.0f - abs(signalSize * 2), 1.0f - abs(signalSize * 2), 1.0f, 1.0f);
                else
                    sphere.render(f, camera, 1.0f, 1.0f - abs(signalSize * 2), 1.0f - abs(signalSize * 2), 1.0f);
            }
            else
                sphere.render(f, camera, colors[nodeColors[node]].R / 255.0f, colors[nodeColors[node]].G / 255.0f, colors[nodeColors[node]].B / 255.0f, colors[nodeColors[node]].A / 255.0f);
        }
        //render text if applicable
        if(shouldRenderText == true)
        {
            screen->renderText(sphere.model, camera, viewport, QString(nodeNames[node].c_str()));
            shouldRenderText = false;
        }
        node++;
    }
    if(paths.size() > 0)
    {
        int pnum = 1;
        for(std::vector<int> path : paths)
        {
            for(int p = 0; p < path.size() - 1; p++)
            {
                //move connector to the spheres location, and then aim it at the connected node
                connector.model = glm::mat4(1);
                glm::vec3 nodePos = glm::vec3(nodePositions[path[p]][3]);
                glm::vec3 conNode = glm::vec3(nodePositions[path[p+1]][3]);
                glm::mat4 look = glm::lookAt(
                    nodePos,                 // position
                    conNode, // looking at
                    glm::vec3(0.0f, 0.0f, 1.0f)                 // up axis
                );
                connector.model *= glm::inverse(look); //apply the lookat trasnformation


                //now scale it so it actually reaches that node
                float dist = glm::distance(nodePos, conNode);
                connector.model = glm::scale(connector.model, glm::vec3(0.5 * connectionSize, 0.5 * connectionSize, dist * 0.5));
                //this line ensures the scale occurs from the BASE of the model
                connector.model *= glm::mat4(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, -1, 1);
                if((mouseDown == 1 && pnum == 3) || mouseDown == 0)
                    connector.render(f, camera, colors[pnum].R, colors[pnum].G, colors[pnum].B, 0.8f);
            }
            pnum++;
        }
    }
    if (displayMri == 1)
    {
        mri.render(f, camera);
    }
    if (displayShell == 1)
    {
        f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        mesh.render(f, camera, colors[11].R / 255.0f, colors[11].G / 255.0f, colors[11].B / 255.0f, colors[11].A / 255.0f);
        f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if(hasTime)
    {
        quint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if(currentTime >= nextFrameTime)
        {
          currentFrame += 0.1f;
          if(currentFrame >= numFrames)
          {
            currentFrame = 0;
          }
          nextFrameTime = currentTime + (*milisecondsPerFrame / 10);
        }
    }
}

float Brain::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float Brain::map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}
