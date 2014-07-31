#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#pragma warning( disable : 4251)

#include "ExportHeader.h"
#include <glm\glm.hpp>

class ENGINE_SHARED DebugCamera
{
private:
	

	void UpdateView();

public:
	glm::vec3 up;	
	glm::vec3 position;
	glm::mat4 view;

	glm::vec3 direction;

	DebugCamera() : position(0,0,0), direction(0,0,1), up(0,1,0){}
	~DebugCamera(){}
	
	void SetPosition(const glm::vec3& pos);
	void SetPosition(float x, float y, float z);
	void SetTarget(const glm::vec3& tar);
	void SetTarget(float x, float y, float z);

	void MoveForward(float speed);
	void MoveSideways(float speed);
	void MoveVertical(float step);
	void RotateHorizontal(float speed);
	void RotateVertical(float speed);

	glm::vec3 GetPosition() const;
	glm::mat4 GetView() const;
	glm::mat4 GetInverseView() const;
};

#endif // DEBUG_CAMERA_H