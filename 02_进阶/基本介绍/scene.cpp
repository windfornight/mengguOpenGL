#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "model.h"
#include "skybox.h"
#include "particlesystem.h"


glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
Ground ground;
Model model, niutou;
SkyBox skybox;
ParticleSystem ps;

void Init()
{
	ground.Init();
	model.Init("Res/Sphere.obj");
	model.SetTexture("Res/earth.bmp");
	model.SetPosition(0.0f, 0.0f, -5.0f);
	skybox.Init("Res/");
	niutou.Init("Res/niutou.obj");
	niutou.SetTexture("Res/niutou.bmp");
	niutou.mModelMatrix = glm::translate(-0.5f, 0.0f, 4.0f) * glm::scale(0.05f, 0.05f, 0.05f);
	ps.Init(0.0f, 0.0f, 0.0f);
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

	skybox.Draw(viewMatrix, projectionMatrix);
	ground.Draw(viewMatrix, projectionMatrix);
	model.Draw(viewMatrix, projectionMatrix);
	niutou.Draw(viewMatrix, projectionMatrix);
	ps.Update(frameTime);
	ps.Draw(viewMatrix, projectionMatrix);
}