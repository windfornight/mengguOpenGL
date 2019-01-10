#include "scene.h"
#include "utils.h"
#include "skybox.h"
#include "model.h"
#include "ground.h"
#include "light.h"
#include "camera.h"
#include "sprite.h"
#include "particle.h"

SkyBox skybox;
Model model;
Ground ground;
DirectionLight light(GL_LIGHT0);
PointLight light1(GL_LIGHT1), light2(GL_LIGHT2);
SpotLight light3(GL_LIGHT3);
Camera camera;
Sprite2D sprite;
GLuint particleTexture;
Particle particle[1000];


void Init()
{
	glMatrixMode(GL_PROJECTION);  //当前矩阵，投影矩阵
	gluPerspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f); //改变当前矩阵
	glMatrixMode(GL_MODELVIEW); //模型视口矩阵
	glLoadIdentity();
	skybox.Init("Res/");
	//model.Init("Res/Quad.obj");
	model.Init("Res/Sphere.obj");
	model.mTexture = CreateTexture2DFromBMP("Res/earth.bmp");
	light.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light.SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	light.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light.SetPosition(0.0f, 1.0f, 0.0f);
	light1.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light1.SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	light1.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light1.setPosition(0.0f, 0.0f, 0.0f);
	light1.SetConstAttenuation(1.0f);
	light1.SetLinearAttenuation(0.2f);
	light2.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light2.SetDiffuseColor(0.1f, 0.4f, 0.6f, 1.0f);
	light2.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light2.setPosition(0.0f, 0.0f, -30.0f);
	light2.SetConstAttenuation(1.0f);
	light2.SetLinearAttenuation(0.2f);

	light3.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light3.SetDiffuseColor(0.0f, 0.8f, 0.0f, 1.0f);
	light3.SetSpecularColor(1.0f, 0.0f, 0.0f, 1.0f);
	light3.setPosition(10.0f, 50.0f, -30.0f);
	light3.SetDirection(0.0f, -1.0f, 0.0f);
	light3.SetExponent(5.0f);
	light3.SetCutoff(10.0f);


	model.SetAmbientMaterial(0.1f, 0.1f, 0.1f, 1.0f);
	model.SetDiffuseMaterial(0.4f, 0.4f, 0.4f, 1.0f);
	model.SetSpecularMaterial(1.0f, 1.0f, 1.0f, 1.0f);
	ground.SetAmbientMaterial(0.1f, 0.1f, 0.1f, 1.0f);
	ground.SetDiffuseMaterial(0.4f, 0.4f, 0.4f, 1.0f);
	ground.SetSpecularMaterial(0.0f, 0.0f, 0.0f, 1.0f);

	camera.mViewportWidth = 800;
	camera.mViewportHeight = 600;

	//sprite.SetImage("Res/front.bmp");
	sprite.SetImage("Res/head.png");
	sprite.SetRect(0.0f, 0.0f, 100.0f, 100.0f);

	particleTexture = CreateProcedureTexture(128);
	
}

void EmitParticle(float delta)
{
	static float currentSleepTime = 0.0f;
	static float nextParticleTime = 0.016f;
	static int particleCount = 1;
	if (particleCount == 1000)
	{
		return;
	}
	currentSleepTime += delta;
	if (currentSleepTime >= nextParticleTime)
	{
		currentSleepTime = 0.0f;
	}
	else
	{
		return;
	}
	particle[particleCount - 1].mHalfSize = 10.0f;
	particle[particleCount - 1].mTexture = particleTexture;
	particle[particleCount - 1].Init(220, 150, 50, 255, 10.0f);
	particleCount++;
}


void Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.SwitchTo3D();
	float frameTime = GetFrameTime();
	camera.Update(frameTime);
	//glEnable(GL_DEPTH_TEST);
	//glLoadIdentity();
	//gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f); //脑袋位置， 眼睛看的视点， 从头顶发出去的方向向量
	//light.Enable();
	light1.Enable();
	light1.Update(camera.mPos.x, camera.mPos.y, camera.mPos.z);
	light2.Enable();
	light2.Update(camera.mPos.x, camera.mPos.y, camera.mPos.z);
	light3.Enable();
	light3.Update(camera.mPos.x, camera.mPos.y, camera.mPos.z);
	skybox.Draw(camera.mPos.x, camera.mPos.y, camera.mPos.z);
	//model.Draw();
	ground.Draw();
	camera.SwitchTo2D();
	sprite.Draw();
	
	EmitParticle(frameTime);
	for (int i = 0; i < 1000; ++i)
	{
		if (particle[i].mLifeTime != -1.0f)
		{
			particle[i].Update(frameTime);
			particle[i].Draw();
		}
		else
		{
			return;
		}
	}
}

void OnKeyDown(char code)
{
	switch (code)
	{
	case 'A':
		camera.mbMoveLeft = true;
		break;
	case 'D':
		camera.mbMoveRight = true;
		break;
	case 'W':
		camera.mbMoveForward = true;
		break;
	case 'S':
		camera.mbMoveBack = true;
		break;
	}
}

void OnKeyUp(char code)
{
	switch (code)
	{
	case 'A':
		camera.mbMoveLeft = false;
		break;
	case 'D':
		camera.mbMoveRight = false;
		break;
	case 'W':
		camera.mbMoveForward = false;
		break;
	case 'S':
		camera.mbMoveBack = false;
		break;
	}
}

void OnMouseMove(float deltaX, float deltaY)
{
	//x = sinx where x->0
	float angleRotateByUp = deltaX / 1000.0f;
	float angleRotateByRight = deltaY / 1000.0f;
	camera.Yaw(-angleRotateByUp);
	camera.Pitch(-angleRotateByRight);
}