#ifndef SPRING_DEMO_H
#define SPRING_DEMO_H

#include <glm\glm.hpp>
#include <DemoWindow.h>
#include <PhysicsParticle_2D.h>
#include <ParticleForceGenerator_2D.h>

class SpringDemo : public DemoWindow
{
private:
	// BASIC SPRING
	TPhysics::Particle_2D p1;
	TPhysics::Particle_2D p2;
	ParticleForce_Spring springForceGenerator_p1;
	ParticleForce_Spring springForceGenerator_p2;

	// ANCHORED SPRING
	glm::vec2 anchorPoint;
	TPhysics::Particle_2D p3;
	ParticleForce_AnchoredSpring anchoredSpringGenerator;

	// BUOYANCY
	TPhysics::Particle_2D p4;
	ParticleForce_Gravity gravityForceGenerator;
	ParticleForce_Buoyancy buoyancyForceGenerator;

	// SPRING-CHAIN
	TPhysics::Particle_2D p5;
	TPhysics::Particle_2D p6;
	TPhysics::Particle_2D p7;
	TPhysics::Particle_2D p8;

	ParticleForce_Spring p5_SpringForce;
	ParticleForce_Spring p6_SpringForce;
	ParticleForce_Spring p7_SpringForce;

	// BUNGEE
	TPhysics::Particle_2D p9;
	TPhysics::Particle_2D p10;

	ParticleForce_Bungee bungeeForce_p9;
	ParticleForce_Bungee bungeeForce_p10;


	ParticleForceRegistry_2D forceRegistry;

	virtual void InitializeDebugGui();
	void ResetParticles();

public:
	SpringDemo() : 
		springForceGenerator_p1(&p1, 1, 5),
		springForceGenerator_p2(&p2, 1, 5),
		anchoredSpringGenerator(&anchorPoint, 1, 3),
		gravityForceGenerator(glm::vec2(0, -20)),
		buoyancyForceGenerator(-1, 1, 0, 500),
		p5_SpringForce(&p5, 1, 1),
		p6_SpringForce(&p6, 1.5f, 1),
		p7_SpringForce(&p7, 2, 1),
		bungeeForce_p9(&p9, 1, 5),
		bungeeForce_p10(&p10, 1, 5)
		{}
	virtual void Update();

	void ResetPoints();
};

#endif // SPRING_DEMO_H