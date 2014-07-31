#ifndef GENERAL_FORCE_DEMO_H
#define GENERAL_FORCE_DEMO_H

#include <glm\glm.hpp>
#include <DemoWindow.h>
#include <PhysicsParticle_2D.h>
#include <ParticleForceGenerator_2D.h>

class GeneralForceDemo : public DemoWindow
{
private:
	float k1;
	float k2;
	TPhysics::Particle_2D p1;
	ParticleForce_Drag dragForceGenerator;
	ParticleForceRegistry_2D forceRegistry;

	virtual void InitializeDebugGui();
	void ResetParticle();

public slots:
	GeneralForceDemo(): dragForceGenerator(0.1f, 0.2f){}
	virtual void Update();

};

#endif // GENERAL_FORCE_DEMO_H