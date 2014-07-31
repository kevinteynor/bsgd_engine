#include "PhysicsParticle2D.h"

#include <glm\glm.hpp>
#include "TypeDefs.h"
#include "UpdateClock.h"

#include <iostream>

using namespace TPhysics;

Particle2D::Particle2D()
{
	Position = glm::vec2(0,0);
	Velocity = glm::vec2(0,0);
	Acceleration = glm::vec2(0,0);

	Damping = 0.9999f;
	InverseMass = 1;
}
void Particle2D::Initialize(glm::vec2 position)
{
	Position = position;
	Velocity = glm::vec2(0,0);
	Acceleration = glm::vec2(0,0);

	Damping = 0.9999f;
	InverseMass = 1;
}

void Particle2D::Update()
{
	float dt = UpdateClock::DeltaTime();

#pragma region UPDATE POSITION

	Position += glm::vec2(Velocity.x * dt, Velocity.y * dt);

#pragma endregion
#pragma region UPDATE VELOCITY

	glm::vec2 totalAcceleration = Acceleration + (accumulatedForces * InverseMass);

	Velocity += glm::vec2(totalAcceleration.x * dt, totalAcceleration.y * dt);
	Velocity *= pow(Damping, dt);

	accumulatedForces = glm::vec2(0,0);

#pragma endregion
}

void Particle2D::AddForce(const glm::vec2& force)
{
	accumulatedForces += force;
}
void Particle2D::AddImpulse(const glm::vec2& impulse)
{
	Velocity += (impulse * InverseMass);
}