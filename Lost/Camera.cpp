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
	if (cameraFollowList.size() > 0)
	{
		fprintf(stdout, " [Camera::~Camera()] Not all cameras were unbound upon camera destruction. Some may still be trying to use it");
	}
}

void Camera::update(double deltaTime)
{
	if (m_GoalTransform)
	{
		lost::Transform2D oldTransform = m_Transform;
		m_Transform.lerp(*m_GoalTransform, fminf(deltaTime / 100.0f, 1.0f));
		m_RotationOffset = lost::lerp(m_RotationOffset, -fmaxf(fminf((m_Transform - oldTransform).position.x / 300.0f, 0.7f), -0.7f), deltaTime / 50.0f);
	}
	else
	{
		fprintf(stderr, " [Camera::update()] Goal transform not found!!!");
	}
}

void Camera::setSize(float w, float h)
{
	m_Bounds.w = w;
	m_Bounds.h = h;
}

void Camera::setScale(lost::Vector2D scale)
{
	m_Transform.scale = scale;
}

void Camera::setViewportTransforms()
{
	lost::Bound2D viewBounds = m_Bounds * m_Transform.scale + m_Transform.position + lost::Vector2D{ 0.0f, 0.141412f };
	viewBounds = viewBounds - lost::Vector2D{ viewBounds.w / 2.0f, viewBounds.h / 2.0f };
    sgp_project(viewBounds.left, viewBounds.right, viewBounds.top, viewBounds.bottom);
	sgp_rotate_at(m_Transform.rotation + m_RotationOffset, m_Transform.position.x + m_Bounds.w / 2.0f, m_Transform.position.y + m_Bounds.h / 2.0f);
}

void Camera::resetViewportTransforms()
{
	sgp_reset_transform();
	sgp_project(0, m_Bounds.w, 0, m_Bounds.h);
}

void Camera::bindGoalTransform(lost::Transform2D* transformPtr, int priority)
{
	cameraFollowList[transformPtr] = priority;

	m_UpdateGoalPtr();
}

void Camera::unbindGoalTransform(lost::Transform2D* transformPtr)
{
	cameraFollowList.erase(transformPtr);

	m_UpdateGoalPtr();
}

bool Camera::hasFocus(lost::Transform2D* transformPtr)
{
	return (m_GoalTransform == transformPtr);
}

void Camera::addPosition(float x, float y)
{
	m_Transform.position.x += x;
	m_Transform.position.y += y;
}

lost::Vector2D Camera::screenToWorld(lost::Vector2D mousePos)
{
	mousePos = (mousePos - lost::Vector2D{ m_Bounds.w / 2.0f, m_Bounds.h / 2.0f }) * m_Transform.scale + m_Transform.position;
	return mousePos;
}

lost::Bound2D Camera::getViewBounds()
{
	lost::Bound2D viewBounds = m_Bounds * (m_Transform.scale + lost::Vector2D{ 0.1f, 0.1f }) + m_Transform.position;
	viewBounds = viewBounds - lost::Vector2D{ viewBounds.w / 2.0f, viewBounds.h / 2.0f };
	return viewBounds;
}

void Camera::m_UpdateGoalPtr()
{
	int highestPriorityFound = -1;
	for (auto const& [transformPtr, priority] : cameraFollowList)
	{
		if (priority > highestPriorityFound)
		{
			highestPriorityFound = priority;
			m_GoalTransform = transformPtr;
		}
	}

}

namespace lost
{

	Camera globalCamera = Camera();

}