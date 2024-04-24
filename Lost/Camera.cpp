#include "Camera.h"

Camera::Camera()
{
	m_Transform = {};
	m_Bounds = { 0, 0, 0, 0 };
}

Camera::Camera(float w, float h)
{
	m_Bounds = { 0, 0, w, h };
}

Camera::Camera(lost::Bound2D bounds)
{
	m_Bounds = bounds;
}

Camera::~Camera()
{
}

void Camera::update(double deltaTime)
{
	if (m_GoalTransform)
	{
		lost::Transform2D oldTransform = m_Transform;
		m_Transform.lerp(*m_GoalTransform, deltaTime / 100.0f);
		m_RotationOffset = lost::lerp(m_RotationOffset, -fmaxf(fminf((m_Transform - oldTransform).position.x / 300.0f, 0.7f), -0.7f), deltaTime / 50.0f);
	}
}

void Camera::setSize(float w, float h)
{
	m_Bounds.w = w;
	m_Bounds.h = h;
}

void Camera::setViewportTransforms()
{
	lost::Bound2D viewBounds = m_Bounds + m_Transform.position;// -lost::Vector2D{ m_Bounds.w / 2.0f, m_Bounds.h / 2.0f };
    sgp_project(viewBounds.left, viewBounds.right, viewBounds.top, viewBounds.bottom);
	sgp_rotate_at(m_Transform.rotation + m_RotationOffset, m_Transform.position.x + m_Bounds.w / 2.0f, m_Transform.position.y + m_Bounds.h / 2.0f);
}

void Camera::bindGoalTransform(lost::Transform2D* transformPtr)
{
	m_GoalTransform = transformPtr;
}
