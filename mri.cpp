#include "mri.h"

MRI::MRI()
{}

MRI::MRI(QOpenGLFunctions_3_2_Core *f) : data(f, "C:/Users/vrdem/Documents/GitHub/Connected/assets/xT1_brain.tif")
{
    plane = Model();
    plane.loadFromObj(f, "C:/Users/vrdem/Documents/GitHub/Connected/assets/plane.obj", 1);
    plane.addTexture(f, 0);

    glm::quat coronalRotation = glm::quat(glm::vec3(0.0f, 1.5708f, 0.0f));
    coronal = coronal * glm::mat4_cast(coronalRotation);

    glm::quat axialRotation = glm::quat(glm::vec3(1.5708f, 0.0f, 1.5708f));
    axial = axial * glm::mat4_cast(axialRotation);

    coronal = glm::translate(coronal, glm::vec3(coronalTrans.x, coronalTrans.y, coronalTrans.z));
    coronal = glm::scale(coronal, glm::vec3(coronalTrans.sx, coronalTrans.sy, coronalTrans.sz));

    axial = glm::translate(axial, glm::vec3(axialTrans.x, axialTrans.y, axialTrans.z));
    axial = glm::scale(axial, glm::vec3(axialTrans.sx, axialTrans.sy, axialTrans.sz));
}

void MRI::render(QOpenGLFunctions_3_2_Core *f, Camera &camera)
{

    coronal = glm::mat4(1);
    axial = glm::mat4(1);

    glm::quat coronalRotation = glm::quat(glm::vec3(0.0f, 1.5708f, 0.0f));
    coronal = coronal * glm::mat4_cast(coronalRotation);

    glm::quat axialRotation = glm::quat(glm::vec3(1.5708f, 0.0f, 1.5708f));
    axial = axial * glm::mat4_cast(axialRotation);

    coronal = glm::translate(coronal, glm::vec3(coronalTrans.x, coronalTrans.y, coronalTrans.z));
    coronal = glm::scale(coronal, glm::vec3(coronalTrans.sx, coronalTrans.sy, coronalTrans.sz));

    axial = glm::translate(axial, glm::vec3(axialTrans.x, axialTrans.y, axialTrans.z));
    axial = glm::scale(axial, glm::vec3(axialTrans.sx, axialTrans.sy, axialTrans.sz));

    plane.model = glm::translate(coronal, glm::vec3(0.0f, coronalPosition + 80, 0.0f));
    plane.layer = map(coronalPosition + 80, -115.23f + 80, 100.23f + 80, 0.0f, 1.0f);
    plane.view = 0;
    plane.render(f, camera);

    plane.model = glm::translate(axial, glm::vec3(0.0f, axialPosition + 90, 0.0f));
    plane.layer = map(axialPosition + 90, -90.23f + 90, 90.23f + 250, 0.0f, 1.0f);
    plane.view = 1;
    plane.render(f, camera);
}

GLuint MRI::loadShader(QOpenGLFunctions_3_2_Core *f, const char *filepath, GLenum type)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        return 0;
    }

    long len;
    if (fseek(file, 0, SEEK_END) != 0 || (len = ftell(file)) == -1L)
    {
        fclose(file);
        return 0;
    }
    rewind(file);

    char *buffer = (char *)malloc(len);
    if (fread(buffer, 1, len, file) != len)
    {
        fclose(file);
        free(buffer);
        return 0;
    }
    fclose(file);

    GLuint shader = f->glCreateShader(type);
    if (shader == 0)
    {
        free(buffer);
        return 0;
    }

    f->glShaderSource(shader, 1, (const char *const *)&buffer, (GLint *)&len);
    free(buffer);
    return shader;
}


MRI::~MRI()
{
}

float MRI::map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}
