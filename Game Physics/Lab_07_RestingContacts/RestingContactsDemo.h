#ifndef RESTING_CONTACTS_DEMO_H
#define RESTING_CONTACTS_DEMO_H

#include <DemoWindow.h>
#include <glm\glm.hpp>
#include <ParticleContact_2D.h>
#include <ParticleForceGenerator_2D.h>
#include <PhysicsParticle_2D.h>

struct AttachedVectorWatchInfo;
struct PointWatchInfo;
struct BoolWatcherInfo;

class RestingContactsDemo : public DemoWindow
{
private:
	bool autostep;
	bool showVelocityVectors;
	BoolWatcherInfo* autostepWatchInfo;
	BoolWatcherInfo* showVelVecsWatchInfo;
	float fps;

	TPhysics::Particle_2D p;
	float p_mass;

	ParticleForce_GroundFriction groundFriction;
	ParticleForceRegistry_2D forceRegistry;

	virtual void InitializeDebugGui();
	void ResetParticles();
	void CheckForCollision();

public:
	RestingContactsDemo() : groundFriction(0.55f, 0.65f), forceRegistry(){}

	void Step();
	virtual void Update();
	void Collide();

};

#endif // RESTING_CONTACTS_DEMO_H