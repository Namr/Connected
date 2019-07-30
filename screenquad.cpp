#include "screenquad.h"

screenQuad::screenQuad()
{

}

void screenQuad::init(QOpenGLFunctions_3_2_Core *f)
{
    // generate and bind the buffers assosiated with this chunk in order to assign
    // vertices and color to the mesh
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);

    // load the shaders from their corresponding files
    GLuint vertexShader = loadShader(f, "shaders/screenVert.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader =
        loadShader(f, "shaders/screenFrag.glsl", GL_FRAGMENT_SHADER);
    // compile the GPU programs
    f->glCompileShader(vertexShader);
    f->glCompileShader(fragmentShader);

    // catch any errors
    GLint success;
    GLchar infoLog[512];

    f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Vertex Shader Compile Fail");
        std::cout << "message: " << infoLog << std::endl;
        messageBox.setFixedSize(500, 200);
    };

    f->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Fragment Shader compile fail");
        messageBox.setFixedSize(500, 200);
    };

    // create a program from the shaders
    shaderProgram = f->glCreateProgram();
    f->glAttachShader(shaderProgram, vertexShader);
    f->glAttachShader(shaderProgram, fragmentShader);

    f->glBindFragDataLocation(shaderProgram, 0, "outColor");

    // finilize the program and use it
    f->glLinkProgram(shaderProgram);
    f->glUseProgram(shaderProgram);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the array buffer to contain sections the size of a Vertex struct, and
    // pass a pointer to the vector containing them
    f->glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float),
        &quadVertices[0], GL_STATIC_DRAW);
    GLuint posAttrib = GLCheckError(f->glGetAttribLocation(shaderProgram, "position"));
    GLuint texcoordsAttrib = GLCheckError(f->glGetAttribLocation(shaderProgram, "texCoords"));
    f->glEnableVertexAttribArray(posAttrib);
    f->glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(texcoordsAttrib);
    f->glVertexAttribPointer(texcoordsAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    uniTexture = f->glGetUniformLocation(shaderProgram, "screenTexture");
    f->glUniform1i(uniTexture, 0);
}

void screenQuad::render(QOpenGLFunctions_3_2_Core *f)
{
    f->glUseProgram(shaderProgram);
    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint screenQuad::loadShader(QOpenGLFunctions_3_2_Core *f, const char *filepath, GLenum type)
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
