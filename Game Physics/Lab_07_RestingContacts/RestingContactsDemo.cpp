#include "RestingContactsDemo.h"
#include <DemoWindow.h>
#include <glm\glm.hpp>
#include <GridWidget_2D.h>
#include <ParticleContact_2D.h>
#include <ParticleForceGenerator_2D.h>
#include <PhysicsParticle_2D.h>
#include <VerticalDebugMenu.h>
#include <UpdateClock.h>

#include <iostream>

void RestingContactsDemo::InitializeDebugGui()
{
	autostep = false;
	showVelocityVectors = true;
	fps = 240;

	forceRegistry.Add(&p, &groundFriction);

	VerticalDebugMenu::WatchBool("Autostep", &autostep);
	DemoWindow::WatchPoint(&p.Position, &p_mass, Qt::red, false);

	VerticalDebugMenu::WatchFloatSlider("FPS", &fps, 10, 480);

	Delegate f = Delegate();
	f.bind(this, &RestingContactsDemo::ResetParticles);
	VerticalDebugMenu::WatchButton("Reset", f);

	ResetParticles();
}

void RestingContactsDemo::Step()
{
	UpdateClock::SetDeltaTime(1 / fps);
	p.Update();
	forceRegistry.UpdateForces();

	std::cout << p.Position.y << std::endl;

	CheckForCollision();
}

void RestingContactsDemo::Update()
{
	if(autostep) Step();

	p_mass = 1.0f / p.InverseMass;
	DemoWindow::Update();
}

void RestingContactsDemo::CheckForCollision()
{
	if(p.Position.y <= 0)
	{
		Collide();
	}
}

void RestingContactsDemo::Collide()
{
	ParticleContact_2D contact = ParticleContact_2D(&p, NULL, 0.7f);
	contact.contactNormal = glm::vec2(0,1);
	contact.penetration = -p.Position.y;
	contact.restitution = 0.65f;
	contact.Resolve();
}

void RestingContactsDemo::ResetParticles()
{
	p.Position = glm::vec2(-10, 10);
	p.Velocity = glm::vec2(0,0);
	p.Acceleration = glm::vec2(0, -10);	// gravity
	p.AddImpulse(glm::vec2(3,0));
}