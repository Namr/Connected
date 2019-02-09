#include "NIFTI.h"

NIFTI::NIFTI() {}
NIFTI::NIFTI(QOpenGLFunctions_4_0_Core *f, std::string filename)
{
	//find the number of layers in the tiff file
	TIFF *temp = TIFFOpen(filename.c_str(), "r");
	if (temp)
	{
		do
		{
			depth++;
		} while (TIFFReadDirectory(temp));
		TIFFClose(temp);
	}

	TIFF *image = TIFFOpen(filename.c_str(), "r");
	if (image)
	{
		TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height);
		uint16_t *buf = (uint16_t *)_TIFFmalloc(width * sizeof(uint16_t));
		data = new uint16_t[height * width * depth];

		for (int layer = 0; layer < depth; layer++)
		{
			for (int row = 0; row < height; row++)
			{
				TIFFReadScanline(image, (tdata_t)buf, row);
				for (int col = 0; col < width; col++)
					data[(layer * width * height) + (row * width) + col] = buf[col];
			}
			TIFFReadDirectory(image);
		}

		_TIFFfree((tdata_t)buf);
		TIFFClose(image);

		//generate the buffers for the texture data which will be supplied by another function
		f->glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_3D, texture);

		//allocate memory for the incoming data and upload it
		f->glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, width, height, depth, 0, GL_RED, GL_UNSIGNED_SHORT, data);

		free(data);

		//Set Texture Parameters
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
		std::cout << "ERROR: IMAGE UNABLE TO LOAD" << std::endl;
}
