#ifndef PARTICLE_CONTACT_H
#define PARTICLE_CONTACT_H

#include <glm\glm.hpp>
#include "ExportHeader.h"
#include "PhysicsParticle_2D.h"
#include "EngineTypeDefs.h"

class ParticleContactResolver_2D;

class ENGINE_SHARED ParticleContact_2D
{
	friend ParticleContactResolver_2D;

public:
	TPhysics::Particle_2D* p1;
	TPhysics::Particle_2D* p2;

	// 1 = perfectly elastic, 0 = completely inelastic
	float restitution; 

	float penetration;

	glm::vec2 contactNormal;

	glm::vec2 particleMovement[2];

	ParticleContact_2D(TPhysics::Particle_2D* a, TPhysics::Particle_2D* b = 0, float restitution = 1.0f);
	
	void Resolve();
	void resolveVelocity();
	void resolveInterpenetration();

protected:
	float calculateSeparatingVelocity() const;
};

class ENGINE_SHARED ParticleContactResolver_2D
{
protected:
	uint iterations;
	uint iterationsUsed;

public:
	ParticleContactResolver_2D(uint i) : iterations(i){}
	void SetIterations(uint i);

	void ResloveContacts(ParticleContact_2D * contactArray, uint numContacts);
};

#endif // PARTICLE_CONTACT_H