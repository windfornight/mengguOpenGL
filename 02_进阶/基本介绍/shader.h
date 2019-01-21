#pragma once
#include "ggl.h"

struct UniformTexture
{
	GLint mLocation;
	GLuint mTexture;
	UniformTexture()
	{
		mLocation = -1;
		mTexture = 0;
	}
};

struct UniformVector4f
{
	GLint mLocation;
	float v[4];
	UniformVector4f()
	{
		mLocation = -1;
		memset(v, 0, sizeof(float) * 4);
	}
};

class Shader
{
public:
	GLuint mProgram;
	GLuint mPosition;
	GLuint mColor;
	GLuint mTexcoord;
	GLuint mNormal;
	std::map<std::string, UniformTexture*> mUniformTextures;
	std::map<std::string, UniformVector4f*> mUniformVec4s;
	GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
	GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
public:
	void Init(const char*vs, const char*fs);
	void Bind(float *M, float *V, float *p);
	void SetTexture(const char *name, const char *imagePath);
	void SetVec4(const char *name, float x, float y, float z, float w);
};