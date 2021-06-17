#include "brain.h"
#include "GLWidget.h"

Brain::Brain()
{

}

Brain::Brain(QOpenGLFunctions_3_2_Core *f, std::string nodePath, QStringList connectionPath)
{

    reloadBrain(nodePath, connectionPath);

    //color intitation for interpolation later on
    blue.R = 0;
    blue.G = 0;
    blue.B = 255;
    red.R = 255;
    red.G = 0;
    red.B = 0;
    white.R = 255;
    white.G = 255;
    white.B = 255;
    black.R = 1;
    black.G = 1;
    black.B = 1;
    position = glm::mat4(1.0f);

    //init all the models for each part of the brain model, these will be rendered many times over to create the brain
    mri = MRI(f);
    sphere = Model();
    mesh = BrainModel();
    mesh.brain = this;
    connector = Model();
    sphere.loadFromObj(f, "assets/sphere.obj", 0);
    mesh.loadFromNV(f, "assets/BrainMesh_ICBM152_smoothed.nv");
    connector.loadFromObj(f, "assets/connector.obj", 0);
}

//get new braindata from node and connection files
void Brain::reloadBrain(std::string nodePath, QStringList connectionPaths)
{
    //delete any previous brain data we had
    nodePositions.clear();
    nodeNames.clear();
    nodeColors.clear();
    nodeSizes.clear();
    connections.clear();
    currentFrame = 0;

    ///////////////////////////
    ////    LOAD .NODE FILE //
    /////////////////////////
    std::ifstream nodeFile;
    nodeFile.open(nodePath);
    std::string line;

    //make sure numFrames is at the local minimum number it can be
    if(!hasAppendedData)
        numFrames = (numFrames > connectionPaths.size() || !hasTime) ? connectionPaths.size() : numFrames;

    hasTime = numFrames > 1 ? true : false;

    //iterate over each line in the node file if it is found, store it in line, and operate on it
    if (nodeFile.is_open())
    {
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
        messageBox.critical(nullptr, "Error", "Node File not found! Check your paths");
        messageBox.setFixedSize(500, 200);
    }

    ////////////////////////////////
    ////   LOAD CONNECTION FILE //
    //////////////////////////////
    ///
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
                for (unsigned int n = 0; n < nodePositions.size(); n++)
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
                //if there is only one datapoint per line, read the file line by line
                //but every n lines start writing into the next nodes connections, since it is
                //just a flattened matrix
                for (unsigned int n = 0; n < nodePositions.size(); n++)
                {
                    std::vector<float> nodesConnections;
                    for (unsigned int c = 0; c < nodePositions.size(); c++)
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
            messageBox.critical(nullptr, "Error", "Edge File not found! Check your paths");
            messageBox.setFixedSize(500, 200);
        }
    }
}

//this function loads graph signal data, which just exists as a nx2 matrix of time and signal on node
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
        float largestValue = 0;
        for(unsigned int i = 0; i < appendedNodeData.size(); i++)
        {
            for(unsigned int w = 0; w < appendedNodeData[0].size(); w++)
            {
                if(abs(appendedNodeData[i][w]) > largestValue)
                    largestValue = appendedNodeData[i][w];
            }
        }
        //now interpolate linearly between these two min and max values
        for(unsigned int i = 0; i < appendedNodeData.size(); i++)
        {
            for(unsigned int w = 0; w < appendedNodeData[0].size(); w++)
            {
                appendedNodeData[i][w] /= largestValue;
            }
        }

        hasAppendedData = true;
        hasTime = numFrames > 1 ? true : false;
        nextFrameTime = QDateTime::currentMSecsSinceEpoch() + (*milisecondsPerFrame / 10);
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Graph Signal Data File not found! Check your paths");
        messageBox.setFixedSize(500, 200);
    }
}

//setter function for the brains overall position
void Brain::setPosition(glm::vec3 pos)
{
    position = glm::mat4(1.0f);
    position = glm::translate(position, pos);

    updatePosition();
}

//helper function that just updates the component meshes position to match the brains overall position
void Brain::updatePosition()
{
    sphere.parentPosition = position;
    mesh.parentPosition = position;
    connector.parentPosition = position;
}

//called every frame to update and render the brain
void Brain::update(QOpenGLFunctions_3_2_Core *f, Camera &camera, float xpos, float ypos, int &selectedNode, int mouseDown)
{
    GLint viewportraw[4];
    f->glGetIntegerv(GL_VIEWPORT, viewportraw);

    glm::vec4 viewport = glm::vec4(viewportraw[0], viewportraw[1], viewportraw[2], viewportraw[3]);

    /////////////////////////////////////////
    // loop through nodes and render them ///
    /////////////////////////////////////////
    int node = 0;
    for (glm::mat4 pos : nodePositions)
    {
        //move sphere to the position in the node file
        sphere.model = pos;
        //check and apply scaling based on file values
        if(isScaling)
            sphere.model = glm::scale(sphere.model, glm::vec3(nodeSize * nodeSizes[node], nodeSize * nodeSizes[node], nodeSize * nodeSizes[node]));
        else
            sphere.model = glm::scale(sphere.model, glm::vec3(nodeSize, nodeSize, nodeSize));


        //convert mouse position to a 3D ray and check if this node has been hit
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

        //for each node that this node is connected to (for this frame), draw a connection
        int connectedNode = 0;
        bool shouldRenderText = false;
        bool canRenderSphere = false;
        int brainFrame = currentFrame > (connections.size() - 1) ? (connections.size() - 1) : currentFrame;
        for (float connection : connections[brainFrame][node])
        {
            //if this statisfies rendering text, change the flag
            if(connection > textThreshold && connectedNode != node)
            {
                shouldRenderText = true;
            }
            if(connection > threshold && connectedNode != node)
            {
                canRenderSphere = true;
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
                if(isConnectionScaling)
                    connector.model = glm::scale(connector.model, glm::vec3(connection * connectionSize, connection * connectionSize, dist * 0.5));
                else
                    connector.model = glm::scale(connector.model, glm::vec3(connectionSize, connectionSize, dist * 0.5));
                //this line ensures the scale occurs from the BASE of the model
                connector.model *= glm::mat4(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, -1, 1);

                //change color based on settings file
                if(connectionStrengthColor)
                {
                    NColor cColor = clerp(blue, red, (connection - threshold) / (1.0 - threshold));
                    connector.render(f, camera, cColor.R / 255.0f, cColor.B / 255.0f, cColor.G / 255.0f, 0.8f);
                }
                else
                {
                    connector.render(f, camera, colors[nodeColors[node]].R / 255.0f, colors[nodeColors[node]].G / 255.0f, colors[nodeColors[node]].B / 255.0f, 0.8f);
                }
            }
            connectedNode++;
        }
        if (hasAppendedData && !displayHeatMap) //if we have appended data, render it
        {
            //linear interpolation between the current singal size and the next frames size
            float signalSize = lerp(appendedNodeData[node][floor(currentFrame)], appendedNodeData[node][ceil(currentFrame)], currentFrame - floor(currentFrame)) ;

            connector.model = glm::mat4(1);
            connector.model = glm::translate(connector.model, glm::vec3(sphere.model[3]));
            glm::quat rot = glm::quat(glm::vec3(1.5708f, 0.0f, 1.5708f));
            connector.model = connector.model * glm::mat4_cast(rot);
            connector.model = glm::scale(connector.model, glm::vec3(0.7f, signalSize * graphSignalSize, 0.7));

            //this line ensures the scale occurs from the BASE of the model
            connector.model *= glm::mat4(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, -1, 1);

            NColor signalColor;
            //color change depending on negative/positive values
            signalColor = clerp(blue, red, (signalSize + 1.0) / 2.0);

            connector.render(f, camera, signalColor.R / 255.0f, signalColor.G / 255.0f, signalColor.B / 255.0f, 1.0f);
        }

        // depending on if the node has been hit, or if heatmap rendering is enabled, change the sphere color on render
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
                float signalSize = lerp(appendedNodeData[node][floor(currentFrame)], appendedNodeData[node][ceil(currentFrame)], currentFrame - floor(currentFrame));
                NColor signalColor = clerp(blue, red, (signalSize + 1.0f) / 2.0f);
                sphere.render(f, camera, signalColor.R / 255.0f, signalColor.G / 255.0f, signalColor.B / 255.0f, 1.0f);
            }
            else if(displayUnusedNodes || canRenderSphere)
            {
                sphere.render(f, camera, colors[nodeColors[node]].R / 255.0f, colors[nodeColors[node]].G / 255.0f, colors[nodeColors[node]].B / 255.0f, colors[nodeColors[node]].A / 255.0f);
            }
        }
        //render text if applicable
        if(shouldRenderText == true)
        {
            screen->renderText(sphere.model, camera, viewport, QString(nodeNames[node].c_str()));
            shouldRenderText = false;
        }
        node++;
    }
    if (displayMri == 1)
    {
        mri.render(f, camera);
    }
    if (displayShell == 1)
    {
        f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        mesh.render(f, camera, colors[13].R / 255.0f, colors[13].G / 255.0f, colors[13].B / 255.0f, colors[13].A / 255.0f);
        f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //advance the currentFrame to continue the animation
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

NColor Brain::rgb2hsv(NColor in)
{
    float fR = in.R / 255.0f;
    float fG = in.G / 255.0f;
    float fB = in.B / 255.0f;

    float fCMax = fmax(fmax(fR, fG), fB);
    float fCMin = fmin(fmin(fR, fG), fB);
    float fDelta = fCMax - fCMin;

    float fH = 0;
    float fS = 0;
    float fV = 0;

    if(fDelta > 0) {
      if(fCMax == fR) {
        fH = 60 * (fmod(((fG - fB) / fDelta), 6));
      } else if(fCMax == fG) {
        fH = 60 * (((fB - fR) / fDelta) + 2);
      } else if(fCMax == fB) {
        fH = 60 * (((fR - fG) / fDelta) + 4);
      }

      if(fCMax > 0) {
        fS = fDelta / fCMax;
      } else {
        fS = 0;
      }

      fV = fCMax;
    } else {
      fH = 0;
      fS = 0;
      fV = fCMax;
    }

    if(fH < 0) {
      fH = 360 + fH;
    }

    NColor out;
    out.R = fH;
    out.G = fS * 255.0f;
    out.B = fV * 255.0f;
    return out;
}

NColor Brain::hsv2rgb(NColor in)
{
    float fH = in.R;
    float fS = in.G / 255.0f;
    float fV = in.B / 255.0f;

    float fR = 0;
    float fG = 0;
    float fB = 0;

    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;

    if(0 <= fHPrime && fHPrime < 1) {
      fR = fC;
      fG = fX;
      fB = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
      fR = fX;
      fG = fC;
      fB = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
      fR = 0;
      fG = fC;
      fB = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
      fR = 0;
      fG = fX;
      fB = fC;
    } else if(4 <= fHPrime && fHPrime < 5) {
      fR = fX;
      fG = 0;
      fB = fC;
    } else if(5 <= fHPrime && fHPrime < 6) {
      fR = fC;
      fG = 0;
      fB = fX;
    } else {
      fR = 0;
      fG = 0;
      fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;

    NColor out;
    out.R = fR * 255.0f;
    out.G = fG * 255.0f;
    out.B = fB * 255.0f;

    return out;
}

NColor Brain::clerp(NColor a, NColor b, float f)
{
    NColor out;
    NColor ha = rgb2hsv(a);
    NColor hb = rgb2hsv(b);

    out.R = lerp(ha.R, hb.R, f);
    out.G = lerp(ha.G, hb.G, f);
    out.B = lerp(ha.B, hb.B, f);


    return hsv2rgb(out);
}

float Brain::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float Brain::map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}
