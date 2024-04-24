#pragma once
#include "Transform2D.h"
#include "../SokolReference.h"
#include <math.h>

class Camera
{
public:
	Camera();
	Camera(float w, float h);
	Camera(lost::Bound2D bounds);
	~Camera();
	
	void update(double deltaTime);

	void setSize(float w, float h);

	void setViewportTransforms();

	void bindGoalTransform(lost::Transform2D* transformPtr);

private:
	lost::Transform2D m_Transform = { 0, 0 };
	lost::Transform2D* m_GoalTransform = nullptr;
	lost::Bound2D m_Bounds;

	float m_RotationOffset = 0.0f;
};

