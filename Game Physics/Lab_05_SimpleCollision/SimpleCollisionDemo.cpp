#include <iostream>

#include "SimpleCollisionDemo.h"
#include <ParticleContact_2D.h>
#include <Qt\qpainter.h>
#include <UpdateClock.h>
#include <VerticalDebugMenu.h>

void SimpleCollisionDemo::InitializeDebugGui()
{
	p1.Damping = 1;
	p2.Damping = 1;
	p1.InverseMass = 1;
	p2.InverseMass = 1;

	p1_mass = p1.Mass();
	p2_mass = p2.Mass();

	VerticalDebugMenu::WatchFloatSlider("P1 Mass", &p1_mass, 0.05f, 10);
	VerticalDebugMenu::WatchFloatSlider("P2 Mass", &p2_mass, 0.05f, 10);

	VerticalDebugMenu::WatchFloatSlider("Restitution", &contact.restitution, 0, 1);
	
	VerticalDebugMenu::WatchFloatSlider("Step Frequency", &fps, 1, 300);
	VerticalDebugMenu::WatchBool("Autostep", &autostep);

	Delegate f = Delegate();
	f.bind(this, &SimpleCollisionDemo::Step);
	VerticalDebugMenu::WatchButton("Step", f);

	f = Delegate();
	f.bind(this, &SimpleCollisionDemo::Collide);
	VerticalDebugMenu::WatchButton("Collide", f);

	f = Delegate();
	f.bind(this, &SimpleCollisionDemo::ResetParticles);
	VerticalDebugMenu::WatchButton("Reset", f);

	DemoWindow::WatchPoint(&p1.Position, &p1.InverseMass, Qt::red);
	DemoWindow::WatchPoint(&p2.Position, &p2.InverseMass, Qt::green);
	DemoWindow::WatchVector(&p1.Velocity, &p1.Position, Qt::blue);
	DemoWindow::WatchVector(&p2.Velocity, &p2.Position, Qt::blue);

	ResetParticles();
}

void SimpleCollisionDemo::Step()
{
	UpdateClock::SetDeltaTime(1 / fps);

	p1.Update();
	p2.Update();

	CheckForCollision();
}

void SimpleCollisionDemo::Update()
{
	p1.InverseMass = 1 / p1_mass;
	p2.InverseMass = 1 / p2_mass;

	if(autostep) Step();

	DemoWindow::Update();
}

void SimpleCollisionDemo::CheckForCollision()
{
	float p1_r = p1.Mass() / 3.0f;	// 3.0 from GridWidget_2D.cpp #define POINT_RADIUS_SCALE
	float p2_r = p2.Mass() / 3.0f;

	if (glm::length(p1.Position - p2.Position) <= (p1_r + p2_r))
		Collide();
}

void SimpleCollisionDemo::Collide()
{
	//contact = ParticleContact_2D(&p1, &p2, 1);
	p1.InverseMass = 1 / p1_mass;
	p2.InverseMass = 1 / p2_mass;

	contact.resolveVelocity();
	DemoWindow::Update();
}

void SimpleCollisionDemo::ResetParticles()
{
	// 45* angle
	//p1.Position = glm::vec2(-5,-5);
	//p2.Position = glm::vec2(5,5);
	//
	//p1.Velocity = glm::vec2(sqrtf(2) / 2.0f,sqrtf(2) / 2.0f);
	//p2.Velocity = -glm::vec2(p1.Velocity);

	// flat
	p1.Position = glm::vec2(-5,0);
	p2.Position = glm::vec2(5,0);
	
	p1.Velocity = glm::vec2(1,0);
	p2.Velocity = glm::vec2(-1,0);//-glm::vec2(p1.Velocity);

	// some random angle
	//p1.Position = glm::vec2(-3,-2);
	//p2.Position = glm::vec2(3,5);
	//
	//p1.Velocity = glm::vec2(3, 2);
	//p2.Velocity = glm::vec2(-1,-3);

	DemoWindow::Update();
}