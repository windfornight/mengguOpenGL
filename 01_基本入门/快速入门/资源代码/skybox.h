#pragma once
#include "ggl.h"

class SkyBox
{
private:
	GLuint mTextures[6];
	GLuint mFastDrawCall;
public:
	void Init(const char *imageDir);
	void DrawCommand();
	void Draw(float x, float y, float z);
};
