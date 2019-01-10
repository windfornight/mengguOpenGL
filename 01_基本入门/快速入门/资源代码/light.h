#pragma once

#include "ggl.h"

class Light
{
protected:
	GLenum mLightIdentifier;
	Light();
public:
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void Enable();
};

class DirectionLight : public Light
{
public:
	DirectionLight(GLenum light);
	void SetPosition(float x, float y, float z);
};

class PointLight : public Light
{
private:
	float mPosition[3];
public:
	PointLight(GLenum light);
	void setPosition(float x, float y, float z);
	void SetConstAttenuation(float v);
	void SetLinearAttenuation(float v);
	void SetQuadricAttenuation(float v);
	void Update(float x, float y, float z);
};

class SpotLight : public PointLight
{
public:
	SpotLight(GLenum light);
	void SetDirection(float x, float y, float z);
	void SetExponent(float v);
	void SetCutoff(float v);
};