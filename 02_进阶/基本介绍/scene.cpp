#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "model.h"


glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
Ground ground;
Model model;

void Init()
{
	ground.Init();
	model.Init("Res/Sphere.obj");
	model.SetPosition(0.0f, 0.0f, -5.0f);
}

void SetViewPortSize(float width, float height)
{
	projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}

void Draw()
{
	float frameTime = GetFrameTime();
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ground.Draw(viewMatrix, projectionMatrix);
	model.Draw(viewMatrix, projectionMatrix);
}