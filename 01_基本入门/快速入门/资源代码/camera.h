#pragma once

#include "vector3f.h"

class Camera
{
public:
	bool mbMoveLeft, mbMoveRight, mbMoveForward, mbMoveBack;
	Vector3f mPos, mViewCenter, mUp;
	int mViewportWidth, mViewportHeight;
	Camera();
	void Update(float deltaTime);
	void Pitch(float angle);
	void Yaw(float angle);
	void RotateView(float angle, float x, float y, float z);
	void SwitchTo3D();
	void SwitchTo2D();
};