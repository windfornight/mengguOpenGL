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

class Shader
{
	GLuint mProgram;
	std::map<std::string, UniformTexture*> mUniformTextures;
	GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
	GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
public:
	void Init(const char*vs, const char*fs);
	void Bind(float *M, float *V, float *p);
	void SetTexture(const char *name, const char *imagePath);
};