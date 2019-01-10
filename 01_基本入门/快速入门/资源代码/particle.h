#pragma once
#include "vector3f.h"

class Particle
{
public:
	GLuint mTexture;
	Vector3f mPosition;
	float mHalfSize;
	float mLifeTime, mLivingTime;
	GLubyte mColor[4];
	Particle();
	void Init(GLubyte r, GLubyte g, GLubyte b, GLubyte a, float life);
	void Draw();
	void Update(float deltaTime);
};