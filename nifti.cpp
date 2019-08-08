#include "nifti.h"

//right now this loads in a tiff file and not a nifti file, needs to either be renamed or redone
NIFTI::NIFTI() {}
NIFTI::NIFTI(QOpenGLFunctions_3_2_Core *f, std::string filename)
{
    //find the number of layers in the tiff file
    TIFF *temp = TIFFOpen(filename.c_str(), "r");
    if (temp)
    {
        depth = 1;
        do
        {
            depth++;
        } while (TIFFReadDirectory(temp));
        TIFFClose(temp);
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "MRI File not found! Check your paths");
        messageBox.setFixedSize(500, 200);
    }

    TIFF *image = TIFFOpen(filename.c_str(), "r");
    if (image)
    {
        TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height);
        if (width > 0 && height > 0 && depth > 0)
        {
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
            f->glGenTextures(1, &texture);
            f->glBindTexture(GL_TEXTURE_3D, texture);

            //allocate memory for the incoming data and upload it
            f->glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, width, height, depth, 0, GL_RED, GL_UNSIGNED_SHORT, data);

            free(data);

            //Set Texture Parameters
            f->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            f->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            f->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            f->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "MRI File not found! Check your paths");
        messageBox.setFixedSize(500, 200);
    }
}
