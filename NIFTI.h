#pragma once

#include <qopenglfunctions_4_0_core.h>
#include <qopenglwidget.h>
#include <qfile.h>

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <tiffio.h>

class NIFTI
{
public:
	int dimNum, width, height, depth, time, bitsPerPixel, bytesPerPixel;
	float voxelWidth, voxelHeight, voxelDepth, vox_offset;
	uint16_t *data;
	GLuint texture;
	NIFTI();
	NIFTI(QOpenGLFunctions_4_0_Core *f, std::string filepath);
};

template <class T>
void endswap(T *objp)
{
	unsigned char *memp = reinterpret_cast<unsigned char *>(objp);
	std::reverse(memp, memp + sizeof(T));
}

