#ifndef COLLISION_POSITION_DEMO_H
#define COLLISION_POSITION_DEMO_H

#include <glm\glm.hpp>
#include <DemoWindow.h>
#include <PhysicsParticle_2D.h>
#include <ParticleContact_2D.h>
#include <ParticleForceGenerator_2D.h>

struct AttachedVectorWatchInfo;
struct BoolWatcherInfo;

class CollisionPositionDemo : public DemoWindow
{
private:
	bool autostep;
	BoolWatcherInfo* autostepWatchInfo;
	bool showVelocityVectors;
	BoolWatcherInfo* showVelocityVectorsInfo;
	float fps;

	float p1_mass, p2_mass;

	TPhysics::Particle_2D p1, p2;
	glm::vec2 contactNormal;
	glm::vec2 p1SepVelocity, p2SepVelocity;

	glm::vec2 p1_interpenDiff, p2_interpenDiff;

	AttachedVectorWatchInfo* p1_velocityWatch;
	AttachedVectorWatchInfo* p2_velocityWatch;
	AttachedVectorWatchInfo* p1_sepVelocityWatch;
	AttachedVectorWatchInfo* p2_sepVelocityWatch;

	AttachedVectorWatchInfo* p1_interpenDiffWatch;
	AttachedVectorWatchInfo* p2_interpenDiffWatch;

	ParticleContact_2D contact;

	virtual void InitializeDebugGui();
	void ResetParticles();
	void CheckForCollision();

public:
	CollisionPositionDemo(void):fps(60), contact(&p1, &p2, 1){}

	void Step();
	virtual void Update();
	void Collide();

};

#endif // COLLISION_POSITION_DEMO_H