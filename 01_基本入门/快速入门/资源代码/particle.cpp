#include "particle.h"
#include "utils.h"

Particle::Particle()
{
	mLifeTime = -1;
}

void Particle::Draw()
{
	glEnable(GL_BLEND);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glDisable(GL_LIGHTING);
	glColor4ubv(mColor);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(mPosition.x - mHalfSize, mPosition.y - mHalfSize, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(mPosition.x + mHalfSize, mPosition.y - mHalfSize, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(mPosition.x + mHalfSize, mPosition.y + mHalfSize, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(mPosition.x - mHalfSize, mPosition.y + mHalfSize, 0.0f);
	glEnd();
	glDisable(GL_BLEND);
}

void Particle::Init(GLubyte r, GLubyte g, GLubyte b, GLubyte a, float life)
{
	mLivingTime = 0.0f;
	mLifeTime = life;
	mColor[0] = r;
	mColor[1] = g;
	mColor[2] = b;
	mColor[3] = a;
}

void Particle::Update(float deltaTime)
{
	if (mLivingTime > mLifeTime)
	{
		return;
	}

	mLivingTime += deltaTime;
	float maxRadius = 300.0f;
	float rotateSpeed = 360.0f;
	float currentAngle = rotateSpeed * mLivingTime;
	float currentRadius = maxRadius * mLivingTime / mLifeTime;
	mPosition.x = currentRadius * cosf(currentAngle * 3.14 / 180.0f);
	mPosition.y = currentRadius * sinf(currentAngle * 3.14 / 180.0f);
}