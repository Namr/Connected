#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"


Model::Model()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	parentPosition = glm::mat4(1.0f);
}

/** Read file into string. */
std::string slurp(const std::string &path)
{
	QFile f(path.c_str());
	return f.readAll().toStdString();
}

void Model::loadFromObj(QOpenGLFunctions_4_0_Core *f, std::string path, int hasTextures)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str()))
	{
		throw std::runtime_error(err);
	}

	for (const auto &shape : shapes)
	{
		for (const auto &index : shape.mesh.indices)
		{
			//add vertices 
			vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
			vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
			vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
			if (hasTextures == 1)
			{
				//add texture coordinates
				vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
				vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
			triangles.push_back(triangles.size());
			triangles.push_back(triangles.size());
			triangles.push_back(triangles.size());
		}
	}
	GLInit(f);
}

void Model::GLInit(QOpenGLFunctions_4_0_Core *f)
{
	// generate and bind the buffers assosiated with this chunk in order to assign
	// vertices and color to the mesh
	f->glGenVertexArrays(1, &VAO);
	f->glGenBuffers(1, &VBO);
	f->glGenBuffers(1, &EBO);

	// load the shaders from their corresponding files
	GLuint vertexShader = loadShader(f, "shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader =
		loadShader(f, "shaders/fragment.glsl", GL_FRAGMENT_SHADER);

	// compile the GPU programs
	f->glCompileShader(vertexShader);
	f->glCompileShader(fragmentShader);

	// catch any errors
	GLint success;
	GLchar infoLog[512];

	f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR VERTEX SHADER COMPILE FAILED\n"
			<< infoLog << std::endl;
	};

	f->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		f->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR FRAGMENT SHADER COMPILE FAILED\n"
			<< infoLog << std::endl;
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
	f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
		&vertices[0], GL_STATIC_DRAW);

	// pass and bind triangle data
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		triangles.size() * sizeof(GLuint), &triangles[0],
		GL_STATIC_DRAW);

	// pass vertex positions to shader program
	GLint posAttrib = f->glGetAttribLocation(shaderProgram, "position");
	f->glEnableVertexAttribArray(posAttrib);
	f->glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	GLint texcoordsAttrib = f->glGetAttribLocation(shaderProgram, "texCoords");
	f->glEnableVertexAttribArray(texcoordsAttrib);
	f->glVertexAttribPointer(texcoordsAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	uniColor = f->glGetUniformLocation(shaderProgram, "objColor");
	uniTrans = f->glGetUniformLocation(shaderProgram, "model");
	uniView = f->glGetUniformLocation(shaderProgram, "view");
	uniProj = f->glGetUniformLocation(shaderProgram, "proj");
	uniParent = f->glGetUniformLocation(shaderProgram, "parentPos");
	uniIsTexOn = f->glGetUniformLocation(shaderProgram, "isTexOn");
	uniTexture = f->glGetUniformLocation(shaderProgram, "textureArray");
	uniLayer = f->glGetUniformLocation(shaderProgram, "layer");
	uniMRIView = f->glGetUniformLocation(shaderProgram, "MRIView");

	f->glUniform4f(uniColor, 1.0f, 0.0f, 0.0f, 1.0f);
	f->glUniform1i(uniIsTexOn, 0);
	f->glUniform1i(uniView, view);
}

void Model::render(QOpenGLFunctions_4_0_Core *f, Camera &camera)
{
	f->glUseProgram(shaderProgram);
	f->glBindVertexArray(VAO);
	f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	f->glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
	f->glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	f->glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
	f->glUniformMatrix4fv(uniParent, 1, GL_FALSE, glm::value_ptr(parentPosition));
	f->glUniform1f(uniLayer, layer);
	f->glUniform1i(uniMRIView, view);

	glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
}

void Model::render(QOpenGLFunctions_4_0_Core *f, Camera &camera, float r, float g, float b, float a)
{
	f->glUseProgram(shaderProgram);
	f->glBindVertexArray(VAO);
	f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	f->glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
	f->glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	f->glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
	f->glUniformMatrix4fv(uniParent, 1, GL_FALSE, glm::value_ptr(parentPosition));

	f->glUniform4f(uniColor, r, g, b, a);

	f->glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
}

void Model::addTexture(QOpenGLFunctions_4_0_Core *f, int id)
{
	f->glUniform1i(uniIsTexOn, 1);
	f->glUniform1i(uniTexture, id);
}

GLuint Model::loadShader(QOpenGLFunctions_4_0_Core *f, const char *filepath, GLenum type)
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
