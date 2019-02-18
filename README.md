# Connected
A modern, fast, and feature-rich human brain connectome viewer

## Features
* Fast and flexible ball-and-stick render of connectome data
* Mutliview display of rendered image
* Node isolation (show only connections to and from a single node)
* Side by Side comparson of two connectome datasets
* Overlay of subject MRI ontop of 3D render of connectome
* Circlegraph view of conncetome data (Work in Progress)

## Planned Features
* Support for 4D (over time) datasets
* Allow user inputed MRI data
* Allow user inputed brain mesh
* Brain mesh slicing
* Mac OS and Linux support
* Support for a wider range of file formats

## Known Issues
* Application sometimes wont start on first try, depending on system
* MRI scale is disproportionate to the 3D render
* Controls are framerate dependent
* Mismatching node and edge files results in a crash

## Usage Guide
The program will start in multiview mode showing the **Primary Brain**. The top, side, and front views can be switched to their 
respective opposite by clicking the middle mouse button over the view (i.e front becomes back). The bottom-right view can be controlled by the user.
Orbit the brain using W,A,S,D. W being up, S being down, A left etc. To only see the user controlled view, click the "Single View" 
button on the left side of the window. To select a node, simply hover over it with the mouse pointer. The name of the last selected 
node appears on the left under "Node Name:". To isolate this node, click and hold the right mouse button. To view the **Primary Brain** 
and **Secondary Brain** side by side, click the "Compare" button on the left.

To display your own connectome data, look in the top right corner and click File->Load Primary/Secondary Connectome.
You will be prompted to select the node and then the edge file for your data, and it should instantly update in the viewer.

To change the Threshold, simply slide the slider labeled "Threshold", the brain mesh can be toggled through the brain mesh toggle, and 
the MRI view is similarly controlled through the MRI checkbox. The Axial and Coronal sliders will determine which slice of the MRI is shown
in respect to the 3D model.