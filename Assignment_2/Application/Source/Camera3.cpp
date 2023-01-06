#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 80;
	static const float ZOOM_SPEED = 80;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	if(Application::IsKeyPressed('A'))
	{
		
		position.y = 60;
		position -= right * ZOOM_SPEED * dt;
		target = position + view;
	}
	if(Application::IsKeyPressed('D'))
	{
		position.y = 60;
		position += right * ZOOM_SPEED * dt;
		target = position + view;	
	}
	if(Application::IsKeyPressed('W'))
	{
		position.y = 60;
		position += view * ZOOM_SPEED * dt;
		target = position + view;
	}
	if(Application::IsKeyPressed('S'))
	{
		position.y = 60;
		position -= view * ZOOM_SPEED * dt;
		target = position + view;
	}

	{
		if (position.x >= 150)
		{
			position.x = 150;
			target = position + view;
		}
		else if (position.x <= -150)
		{
			position.x = -150;
			target = position + view;
		}
		else if (position.z <= -150)
		{
			position.z = -150;
			target = position + view;
		}
		else if (position.z >= 150)
		{
			position.z = 150;
			target = position + view;
		}
	}

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		Mtx44 rotation;
		float pitch = CAMERA_SPEED * static_cast<float>(dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		Mtx44 rotation;
		float pitch = -CAMERA_SPEED * static_cast<float>(dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_LEFT))
		{
			Mtx44 rotation;
			float yaw = CAMERA_SPEED * static_cast<float>(dt);
			rotation.SetToRotation(yaw, 0, 1, 0);
			up = rotation * up;
			view = rotation * view;
			target = position + view;

		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			Mtx44 rotation;
			float yaw = -CAMERA_SPEED * static_cast<float>(dt);
			rotation.SetToRotation(yaw, 0, 1, 0);
			up = rotation * up;
			view = rotation * view;
			target = position + view;

		}
	
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp ;
}