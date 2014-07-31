#ifndef PARTICLEFORCEGENERATOR_2D
#define PARTICLEFORCEGENERATOR_2D

#include <glm\glm.hpp>
#include <vector>
#include "ExportHeader.h"
namespace TPhysics { class Particle2D; }

class ENGINE_SHARED ParticleForceGenerator_2D
{
public:
	virtual void UpdateForce(TPhysics::Particle2D* particle) = 0;
};

class ENGINE_SHARED ParticleForce_Gravity : public ParticleForceGenerator_2D
{
	glm::vec2 gravity;

public:
	ParticleForce_Gravity(const glm::vec2 &gravity);
	virtual void UpdateForce(TPhysics::Particle2D* particle);
};

class ENGINE_SHARED ParticleForce_Drag : public ParticleForceGenerator_2D
{
public:
	float k1;	// velocity drag coefficient
	float k2;	// velocity^2 drag coefficient

	ParticleForce_Drag(float k1 = 0.5f, float k2= 0.5f);
	virtual void UpdateForce(TPhysics::Particle2D* particle);

};

class ENGINE_SHARED ParticleForce_GroundFriction : public ParticleForceGenerator_2D
{
public:
	float k1;
	float k2;

	float y;

	ParticleForce_GroundFriction(float k1 = 0.5f, float k2 = 0.5f, float y = 0);
	virtual void UpdateForce(TPhysics::Particle2D* particle);
};

class ENGINE_SHARED ParticleForce_Spring : public ParticleForceGenerator_2D
{
public:
	TPhysics::Particle2D* otherParticle;

	float springConstant;
	float restLength;

	ParticleForce_Spring(TPhysics::Particle2D* other, float springConstant, float restLength);
	virtual void UpdateForce(TPhysics::Particle2D* particle);

};

class ENGINE_SHARED ParticleForce_AnchoredSpring : public ParticleForceGenerator_2D
{
public:
	glm::vec2* anchor;
	
	float springConstant;
	float restLength;

	ParticleForce_AnchoredSpring(glm::vec2* anchor, float springConstant, float restLength);
	virtual void UpdateForce(TPhysics::Particle2D* particle);
};

class ENGINE_SHARED ParticleForce_Bungee : public ParticleForceGenerator_2D
{
public:
	TPhysics::Particle2D* other;

	float springConstant;
	float restLength;

	ParticleForce_Bungee(TPhysics::Particle2D* other = 0, float springConstant = 0, float restLength = 0);
	virtual void UpdateForce(TPhysics::Particle2D* particle);
};

class ENGINE_SHARED ParticleForce_Buoyancy : public ParticleForceGenerator_2D
{
public:
	float maxDepth;
	float volume;
	float liquidHeight;
	float liquidDensity;

	ParticleForce_Buoyancy(float maxDepth, float volume, float liquidHeight, float liquidDensity = 1000.0f);
	virtual void UpdateForce(TPhysics::Particle2D* particle);
};

class ENGINE_SHARED ParticleForceRegistry_2D
{
protected:
	struct ParticleForceRegistration
	{
		TPhysics::Particle2D* particle;
		ParticleForceGenerator_2D* forceGenerator;

		ParticleForceRegistration(TPhysics::Particle2D* part, ParticleForceGenerator_2D* gen)
		{
			particle = part;
			forceGenerator = gen;
		}
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	void Add(TPhysics::Particle2D* particle, ParticleForceGenerator_2D* forceGenerator);
	void Remove(TPhysics::Particle2D* particle, ParticleForceGenerator_2D* forceGenerator);
	void Clear();
	void UpdateForces();

};

#endif PARTICLEFORCEGENERATOR_2D