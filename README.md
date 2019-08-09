# Connected
A modern, fast, and feature-rich human brain connectome viewer

![Connected Brain Viewer](https://github.com/Namr/Connected/tree/master/shaders/connected.png)

## Features
* Fast and flexible ball-and-stick render of connectome data
* Mutliview display of rendered image
* Node isolation (show only connections to and from a single node)
* Side by Side comparson of two connectome datasets
* Overlay of subject MRI ontop of 3D render of connectome
* Overlay of graph signal ontop of 3D render of connectome
* Circlegraph view of conncetome data (Work in Progress)

## Planned Features
* Allow user inputed MRI data
* Brain mesh slicing
* Mac OS support
* Support for a wider range of file formats

## Known Issues
* Mismatching node and edge files results in a crash
* If the number of nodes in your graph signal file and node file differ, it will result in a crash

## Supported File Formats
* Node position and name data should be in a plaintext format whose filenames end with .node
* each node should be declared in the format seen below, and each of these node declarations should be on new lines
* > -38.65	-5.68	50.94	4	4.96	PreCG.L
* Connection data should be in plaintext .edge files, and the connections strength should be between 0 and 1
* The edge files should have one number per line (and be nodeSize * nodeSize in length where nodeSize is the number of nodes in your .node file)
* Edge files can also be in the format where each line has nodeSize many data points, and the file has nodeSize number of lines
* graph signal data should be in a plaintext file, with one number per line, and it should have nodeSize number of lines
* Brain Mesh Files should be in the .nv format

## Usage Guide
### Navigation and Camera Controls
The program will start in multiview mode showing the **Primary Brain**. The top, side, and front views can be switched to their 
respective opposite by clicking the middle mouse button over the view (i.e front becomes back). The bottom-right view can be controlled by the user. The brain mesh can be toggled through the brain mesh checkbox seen at the bottom left of the window, and the MRI view is similarly controlled through the MRI checkbox. The Axial and Coronal sliders will determine which slice of the MRI is shown
in respect to the 3D model.

Orbit the brain using W,A,S,D. W being up, S being down, A left etc. To only see the user controlled view, click the "Single View" 
button on the left side of the window. To select a node, simply hover over it with the mouse pointer. The name of the last selected 
node appears on the left under "Node Name:". To isolate this node, click and hold the right mouse button. To view the **Primary Brain** 
and **Secondary Brain** side by side, click the "Compare" button on the left. Data for the **Primary Brain** and **Secondary Brain** have to be loaded seperately using their respective prompts in the **File** menu at the top of the screen.

### Loading Data
To display your own connectome data, look in the top right corner and click File->Load Primary/Secondary Connectome. Multiple edge files can be selected in this menu to play them over time.
You will be prompted to select the node and then the edge file for your data, and it should instantly update in the viewer. The same can be done for loading in a new Brain Mesh or Graph signal using their respective prompts in the File menu.

### Customizing the network
There are several aspects of the network that can be changed by the user. Using the **Prefrences Menu** at the top of the screen.
#### Colors
Under Prefrences->Color Settings is a menu that controls all the colors used in the viewer. Select which color you would like to change i.e Node Color 6 (the color that corresponds to node group 6 in the .node file). Once a color is selected from the left hand side, its current shade will appear in a large box in the center of the window. The RGBA values can be changed through the corrisponding sliders underneath. The changes happen instantly, so its a good idea to look at the render while playing with the sliders.
#### Network Settings
Under Prefrences->Network Settings is a menu that controls many aspects of network rendering. The Node, Connection, and Graph Signal Sliders will change the size of the rendered nodes in the viewer. There are two ways of doing this, if "Scale to Node Sizes in File" is selected, the value of the slider becomes a multiplier ontop of the size value inside the .node file, if the box is unchecked, all nodes are a fixed size. Node Threshold determines which connections should be displayed. The slider sets the minimum connection value a connection must have if it is to be displayed. Likewise the Node Text Display Threshold is used to determine which nodes should have their name be dispalyed ontop of the renderer. Any node with a connection of a strength greater than the slider setting will have its name rendered. The size of this rendered text can be set using the Node Text Size slider.
#### MRI Settings
Under Prefrences->MRI Settings is a menu that controls the scale and position of the MRI overlay. The X Y and Z origin of the MRI can be changed under the Position tab, and the X Y Z Scale can be changed under the Scale Tab. You can use the radio buttons on the left hand side to change which MRI is being customized.

### Project Files
To save the custom settings for your network, File->Save Project will open a prompt to select a file name and location for your save file. It is recommended that you end your file name in ".ini". This file can be reloaded through File->Load Project, and all your custom settings will be brought back into the viewer.
