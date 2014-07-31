#ifndef SIMPLE_COLLISION_DEMO_H
#define SIMPLE_COLLISION_DEMO_H

#include <glm\glm.hpp>
#include <DemoWindow.h>
#include <PhysicsParticle_2D.h>
#include <ParticleContact_2D.h>
#include <ParticleForceGenerator_2D.h>

class SimpleCollisionDemo : public DemoWindow
{
private:
	bool autostep;
	float fps;

	float p1_mass, p2_mass;

	TPhysics::Particle_2D p1, p2;

	ParticleContact_2D contact;

	virtual void InitializeDebugGui();
	void ResetParticles();
	void CheckForCollision();

public:
	SimpleCollisionDemo():fps(60), contact(&p1, &p2, 1){}
	void Step();
	virtual void Update();

	void Collide();
};

#endif // SIMPLE_COLLISION_DEMO_H