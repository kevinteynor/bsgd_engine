#include <glm\gtx\transform.hpp>
#include <cmath>
#include "DebugCamera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

void DebugCamera::SetPosition(const vec3& pos)
{
	position = pos;
	
	UpdateView();
}
void DebugCamera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	UpdateView();
}
void DebugCamera::SetTarget(const vec3& pos)
{
	direction = glm::normalize(pos);
	UpdateView();
}
void DebugCamera::SetTarget(float x, float y, float z)
{
	direction = glm::normalize(vec3(x,y,z));

	UpdateView();
}

void DebugCamera::MoveForward(float speed)
{
	position = position + (speed * direction);

	UpdateView();
}
void DebugCamera::MoveSideways(float speed)
{
	vec3 cross = glm::normalize(glm::cross(up, direction));
	position = position + (cross * speed);
	
	UpdateView();
}
void DebugCamera::MoveVertical(float step)
{
	position = position + (step * up);

	UpdateView();
}
void DebugCamera::RotateHorizontal(float speed)
{
	mat4 rotation = glm::rotate(speed, up);
	direction = vec3(rotation * vec4(direction, 1));
	
	UpdateView();
}
void DebugCamera::RotateVertical(float speed)
{
	vec3 cross = glm::cross(direction, up);

	mat4 rotation = glm::rotate(speed, cross);
	direction = vec3(rotation * vec4(direction, 0));

	UpdateView();
}
void DebugCamera::UpdateView()
{
	view = glm::lookAt(position, position + direction, up);
} 

vec3 DebugCamera::GetPosition() const
{
	return position;
}
mat4 DebugCamera::GetView() const
{
	return view;
}
mat4 DebugCamera::GetInverseView() const
{
	return glm::inverse(view);
}