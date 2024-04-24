#pragma once
#include "Transform2D.h"
#include "../SokolReference.h"
#include <math.h>
#include <map>

class Camera
{
public:
	Camera();
	Camera(float w, float h);
	Camera(lost::Bound2D bounds);
	~Camera();
	
	// Updates the camera's transform, lerping towards it's goal
	void update(double deltaTime);

	// Sets the size of the camera's viewport
	void setSize(float w, float h);
	// Set scale of the camera's viewport;
	void setScale(lost::Vector2D scale);

	// Is ran before drawing world, sets the viewports transforms to match the cameras
	void setViewportTransforms();

	// Adds a goal transform to the camera, if it's priority is the highest, it gets control
	void bindGoalTransform(lost::Transform2D* transformPtr, int priority);
	// Removes a goal transform from the camera
	void unbindGoalTransform(lost::Transform2D* transformPtr);

	lost::Vector2D screenToWorld(lost::Vector2D mousePos);

private:
	void m_UpdateGoalPtr();

	lost::Transform2D m_Transform = { 0, 0 };

	lost::Transform2D* m_GoalTransform = nullptr;
	std::map<lost::Transform2D*, int> cameraFollowList;

	lost::Bound2D m_Bounds;

	float m_RotationOffset = 0.0f;
};

namespace lost
{
	
	extern Camera globalCamera;

}