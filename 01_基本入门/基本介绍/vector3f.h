#pragma once

#include <math.h>

class Vector3f
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};

	Vector3f(float x, float y, float z);
	Vector3f();
	Vector3f operator*(float scaler);  
	float operator*(const Vector3f &r);
	Vector3f operator^(Vector3f &r);
	Vector3f operator+(const Vector3f &r);
	Vector3f operator-(Vector3f &r);
	void operator=(const Vector3f &r);  //注意加const的意义, 中间变量
	void Normalize();
	float Magnitude();

};