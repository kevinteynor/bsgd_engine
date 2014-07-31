#ifndef CLOTH_DEMO_H
#define CLOTH_DEMO_H

#include <DemoWindow.h>
#include <glm\glm.hpp>
#include "PhysicsParticle2D.h"
#include "ParticleContact2D.h"
#include "ParticleForceGenerator_2D.h"

class ClothP
{
public:
	float imass;
	glm::vec2 position;
	glm::vec2 lastPosition;
	glm::vec2 acceleration;

	ClothP()
	{
		imass = 1;
		position = glm::vec2(0,0);
		lastPosition = glm::vec2(0,0);
		acceleration = glm::vec2(0,0);
	}
	void Update(float timestepSq)
	{
		glm::vec2 velocity = position - lastPosition;

		glm::vec2 newPosition = position + velocity + (acceleration * timestepSq);

		lastPosition = position;

		position = newPosition;
	}
};
class ClothLink
{
public:
	float* restDistance, * tearDistance, * stiffness;

	ClothP* p1, * p2;

	ClothLink()
	{
		restDistance = 0;
		tearDistance = 0;
		stiffness = 0;
		p1 = 0;
		p2 = 0;
	}
	ClothLink(ClothP* a, ClothP* b, float* rest, float* tear, float* stiff)
		: p1(a), p2(b), restDistance(rest), tearDistance(tear), stiffness(stiff){}
	void Solve()
	{
		if(!p1 || !p2) return;

		glm::vec2 diff = p1->position - p2->position;
		float d = glm::length(diff);
		if(d == 0) d = 1;

		float difference = (*restDistance - d) / d * (*stiffness);

		glm::vec2 translate = glm::vec2(diff.x * 0.5 * difference, diff.y * 0.5 * difference);

		p1->position += translate;
		p2->position -= translate;
	}
};

class ClothDemo : public DemoWindow
{
private:
	static const uint DIMENSION = 8;
	float frameSolvesPerFrame;

	float stiffness, restlength, tearlength;

	ClothP points[DIMENSION][DIMENSION];
	ClothLink links_left[DIMENSION][DIMENSION];
	ClothLink links_up[DIMENSION][DIMENSION];

	virtual void InitializeDebugGui();
	void ResetParticles();

public:
	ClothDemo(){}

	void Step();
	virtual void Update();

};

#endif // CLOTH_DEMO_H