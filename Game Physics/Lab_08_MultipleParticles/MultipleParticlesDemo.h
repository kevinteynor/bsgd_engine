#ifndef MULTIPLE_PARTICLES_DEMO_H
#define MULTIPLE_PARTICLES_DEMO_H

#include <DemoWindow.h>
#include <glm\glm.hpp>
#include <ParticleContact_2D.h>
#include <ParticleForceGenerator_2D.h>
#include <PhysicsParticle_2D.h>

struct AttachedVectorWatchInfo;
struct PointWatchInfo;
struct BoolWatcherInfo;


class MultipleParticlesDemo : public DemoWindow
{
private:
	static const uint NUM_BOXES = 14;
	TPhysics::Particle_2D p;
	TPhysics::Particle_2D boxes[NUM_BOXES];

	float fps;
	bool autostep;

	ParticleForce_GroundFriction p_groundFriction;
	ParticleForce_GroundFriction b_groundFriction;
	ParticleForceRegistry_2D forceRegistry;

	virtual void InitializeDebugGui();
	void ResetParticles();
	void CheckForCollision();

public:
	MultipleParticlesDemo() : p_groundFriction(0.55f, 0.65f), b_groundFriction(0.55f, 0.65f), forceRegistry(){}

	void Step();
	virtual void Update();
	void Collide();
};

#endif // MULTIPLE_PARTICLES_DEMO_H