#include <algorithm>
#include "EngineTypeDefs.h"
#include "ParticleForceGenerator_2D.h"
#include "PhysicsParticle2D.h"
#include "TypeDefs.h"
#include "UpdateClock.h"

using TPhysics::Particle2D;

#pragma region PARTICLE_FORCE_GRAVITY

ParticleForce_Gravity::ParticleForce_Gravity(const glm::vec2& gravity)
{
	this->gravity = gravity;
}
void ParticleForce_Gravity::UpdateForce(Particle2D* particle)
{
	if(!particle->HasFiniteMass()) return;
	
	particle->AddForce(gravity * glm::vec2(particle->Mass()));
}

#pragma endregion

#pragma region PARTICLE_FORCE_DRAG
ParticleForce_Drag::ParticleForce_Drag(float k1, float k2)
{
	this->k1 = k1;
	this->k2 = k2;
}

void ParticleForce_Drag::UpdateForce(TPhysics::Particle2D* particle)
{
	
	glm::vec2 force(particle->Velocity.x, particle->Velocity.y);

	float dragCoeff = glm::length(force);

	if(dragCoeff <= 0)
		return;

	dragCoeff = (k1 * dragCoeff) + (k2 * dragCoeff * dragCoeff);

	force = glm::normalize(force);
	force *= -dragCoeff;
	particle->AddForce(force);
}
#pragma endregion

#pragma region PARTICLE_FORCE_GROUNDFRICTION
ParticleForce_GroundFriction::ParticleForce_GroundFriction(float k1, float k2, float y)
{
	this->k1 = k1;
	this->k2 = k2;
	this->y = y;
}

void ParticleForce_GroundFriction::UpdateForce(TPhysics::Particle2D* particle)
{
	if(particle->Position.y > y)
		return;

	glm::vec2 force(particle->Velocity.x, 0);
	float dragCoeff = glm::length(force);

	if(dragCoeff <= 0)
		return;

	dragCoeff = (k1 * dragCoeff) + (k2 * dragCoeff * dragCoeff);

	force = glm::normalize(force);
	force *= -dragCoeff;
	particle->AddForce(force);
}
#pragma endregion

#pragma region PARTICLE_FORCE_SPRING
ParticleForce_Spring::ParticleForce_Spring(TPhysics::Particle2D* other, float springConstant, float restLength)
{
	this->otherParticle = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}
void ParticleForce_Spring::UpdateForce(Particle2D* particle)
{
	
	glm::vec2 force = particle->Position - otherParticle->Position;

	float magnitude = glm::length(force);
	//magnitude = float_abs(magnitude - restLength);
	magnitude = magnitude - restLength;
	magnitude *= springConstant;

	force = glm::normalize(force);
	force *= -magnitude;
	particle->AddForce(force);
}
#pragma endregion

#pragma region PARTICLE_FORCE_ANCHORED_SPRING

ParticleForce_AnchoredSpring::ParticleForce_AnchoredSpring(glm::vec2* anchor, float springConstant, float restLength)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLength;
}
void ParticleForce_AnchoredSpring::UpdateForce(Particle2D* particle)
{
	
	glm::vec2 force = particle->Position - *anchor;

	//float magnitude = float_abs(glm::length(force) - restLength) * springConstant;
	float magnitude = glm::length(force);
	if(magnitude <= restLength) return;

	force = glm::normalize(force);
	force *= -magnitude;
	particle->AddForce(force);
}

#pragma endregion

#pragma region PARTICLE_FORCE_BUNGEE
ParticleForce_Bungee::ParticleForce_Bungee(TPhysics::Particle2D* other, float springConstant, float restLength)
	: other(other), springConstant(springConstant), restLength(restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}
void ParticleForce_Bungee::UpdateForce(TPhysics::Particle2D* particle)
{
	glm::vec2 force = particle->Position;
	force -= other->Position;

	float magnitude = glm::length(force);
	if(magnitude <= restLength)
		return;

	magnitude = springConstant * (restLength - magnitude);

	force = glm::normalize(force);
	force *= magnitude;
	particle->AddForce(force);
}
#pragma endregion

#pragma region PARTICLE_FORCE_BUOYANCY

ParticleForce_Buoyancy::ParticleForce_Buoyancy(float maxDepth, float volume, float liquidHeight, float liquidDensity)
{
	this->maxDepth = maxDepth;
	this->volume = volume;
	this->liquidHeight = liquidHeight;
	this->liquidDensity = liquidDensity;
}
void ParticleForce_Buoyancy::UpdateForce(Particle2D* particle)
{
	
	float depth = particle->Position.y;

	if(depth >= liquidHeight + maxDepth) return;

	glm::vec2 force(0,0);

	if(depth <= liquidHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		particle->AddForce(force);
		return;
	}

	else
	{
		force.y = liquidDensity * volume * (depth - maxDepth - liquidHeight) / 2 * maxDepth;
		particle->AddForce(force);
		return;
	}
}

#pragma endregion

#pragma region PARTICLE_FORCE_REGISTRY
void ParticleForceRegistry_2D::Add(Particle2D* particle, ParticleForceGenerator_2D* forceGenerator)
{
	registrations.push_back(ParticleForceRegistration(particle, forceGenerator));
}

void ParticleForceRegistry_2D::Remove(Particle2D* particle, ParticleForceGenerator_2D* forceGenerator)
{
	ParticleForceRegistration pfr(particle, forceGenerator);

	Registry::iterator i = registrations.begin();
	for(uint k = 0; k < registrations.size(); k++)
	{
		if((i + k)->particle == particle && (i + k)->forceGenerator == forceGenerator)
			registrations.erase(k + i);
	}
}

void ParticleForceRegistry_2D::Clear()
{
	registrations.clear();
}

void ParticleForceRegistry_2D::UpdateForces()
{
	for(Registry::iterator i = registrations.begin(); i != registrations.end(); i++)
	{
		i->forceGenerator->UpdateForce(i->particle);
	}
}
#pragma endregion