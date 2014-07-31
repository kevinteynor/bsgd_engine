#ifndef PARTICLE_CONTACT_H
#define PARTICLE_CONTACT_H

#include <glm\glm.hpp>
#include "ExportHeader.h"
#include "PhysicsParticle2D.h"
#include "EngineTypeDefs.h"

class ParticleContactResolver2D;

class ENGINE_SHARED ParticleContact2D
{
	friend ParticleContactResolver2D;

public:
	TPhysics::Particle2D* p1;
	TPhysics::Particle2D* p2;

	// 1 = perfectly elastic, 0 = completely inelastic
	float restitution; 

	float penetration;

	glm::vec2 contactNormal;

	glm::vec2 particleMovement[2];

	ParticleContact2D(TPhysics::Particle2D* a, TPhysics::Particle2D* b = 0, float restitution = 1.0f);
	
	void Resolve();
	void resolveVelocity();
	void resolveInterpenetration();

protected:
	float calculateSeparatingVelocity() const;
};

class ENGINE_SHARED ParticleContactResolver2D
{
protected:
	uint iterations;
	uint iterationsUsed;

public:
	ParticleContactResolver2D(uint i) : iterations(i){}
	void SetIterations(uint i);

	void ResloveContacts(ParticleContact2D * contactArray, uint numContacts);
};

#endif // PARTICLE_CONTACT_H