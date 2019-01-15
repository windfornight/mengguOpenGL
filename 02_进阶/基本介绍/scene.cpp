#include "scene.h"
#include "ggl.h"
#include "utils.h"

GLuint vbo;
GLuint program;
GLint positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

void Init()
{

}

void SetViewPortSize(float width, float height)
{
	projectionMatrix = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
}

void Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}