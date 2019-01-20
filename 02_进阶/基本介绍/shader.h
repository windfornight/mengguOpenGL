#pragma once
#include "ggl.h"

class Shader
{
	GLuint mProgram;
	GLuint mPosition;
	GLuint mColor;
	GLuint mTexcoord;
	GLuint mNormal;
public:
	GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
	GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
	void Init(const char*vs, const char*fs);
	void Bind(float *M, float *V, float *p);
};