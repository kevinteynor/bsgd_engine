#include "CollisionPositionDemo.h"
#include <iostream>
#include <GridWidget_2D.h>
#include <ParticleContact_2D.h>
#include <Qt\qcheckbox.h>
#include <Qt\qpainter.h>
#include <UpdateClock.h>
#include <VerticalDebugMenu.h>

void CollisionPositionDemo::InitializeDebugGui()
{
	p1.Damping = 1;
	p2.Damping = 1;
	p1.InverseMass = 1;
	p2.InverseMass = 1;

	p1_interpenDiff = glm::vec2(0,0);
	p2_interpenDiff = glm::vec2(0,0);

	p1_mass = p1.Mass();
	p2_mass = p2.Mass();

	showVelocityVectors = true;

	VerticalDebugMenu::WatchFloatSlider("P1 Mass", &p1_mass, 0.05f, 100);
	VerticalDebugMenu::WatchFloatSlider("P2 Mass", &p2_mass, 0.05f, 100);
	VerticalDebugMenu::WatchFloatSlider("Restitution", &contact.restitution, 0, 1);
	VerticalDebugMenu::WatchFloatSlider("Step Frequency", &fps, 1, 300);
	autostepWatchInfo = VerticalDebugMenu::WatchBool("Autostep", &autostep);

	Delegate f = Delegate();
	f.bind(this, &CollisionPositionDemo::Step);
	VerticalDebugMenu::WatchButton("Step", f);

	f = Delegate();
	f.bind(this, &CollisionPositionDemo::Collide);
	VerticalDebugMenu::WatchButton("Collide", f);

	f = Delegate();
	f.bind(this, &CollisionPositionDemo::ResetParticles);
	VerticalDebugMenu::WatchButton("Reset", f);

	DemoWindow::WatchPoint(&p1.Position, &p1.InverseMass, Qt::yellow);
	DemoWindow::WatchPoint(&p2.Position, &p2.InverseMass, Qt::green);
	p1_velocityWatch = DemoWindow::WatchVector(&p1.Velocity, &p1.Position, Qt::blue);
	p2_velocityWatch = DemoWindow::WatchVector(&p2.Velocity, &p2.Position, Qt::blue);
	p1_sepVelocityWatch = DemoWindow::WatchVector(&p1SepVelocity, &p1.Position, Qt::magenta);
	p2_sepVelocityWatch = DemoWindow::WatchVector(&p2SepVelocity, &p2.Position, Qt::magenta);
	p1_interpenDiffWatch = DemoWindow::WatchVector(&p1_interpenDiff, &p1.Position, Qt::red);
	p2_interpenDiffWatch = DemoWindow::WatchVector(&p2_interpenDiff, &p2.Position, Qt::red);

	p1_interpenDiffWatch->visible = false;
	p2_interpenDiffWatch->visible = false;
	

	ResetParticles();
}

void CollisionPositionDemo::Step()
{
	UpdateClock::SetDeltaTime(1 / fps);

	if(p1_interpenDiff != glm::vec2(0,0) || p2_interpenDiff != glm::vec2(0,0))
	{
		p1.Position += p1_interpenDiff;
		p2.Position += p2_interpenDiff;

		p1_interpenDiff = glm::vec2(0,0);
		p2_interpenDiff = glm::vec2(0,0);

		showVelocityVectors = true;
		p1_interpenDiffWatch->visible = false;
		p2_interpenDiffWatch->visible = false;

		contact.resolveVelocity();
		contact.penetration = 0;
	}

	p1.Update();
	p2.Update();

	CheckForCollision();

	contactNormal = glm::normalize(p1.Position - p2.Position);

	p1SepVelocity = contactNormal * glm::dot(p1.Velocity, contactNormal);
	p2SepVelocity = contactNormal * glm::dot(p2.Velocity, contactNormal);
}

void CollisionPositionDemo::Update()
{
	p1.InverseMass = 1 / p1_mass;
	p2.InverseMass = 1 / p2_mass;

	if(autostep) Step();

	if(showVelocityVectors && !p1_velocityWatch->visible)
	{
		p1_velocityWatch->visible = true;
		p2_velocityWatch->visible = true;
		p1_sepVelocityWatch->visible = true;
		p2_sepVelocityWatch->visible = true;
	}
	else if(!showVelocityVectors && p1_velocityWatch->visible)
	{
		p1_velocityWatch->visible = false;
		p2_velocityWatch->visible = false;
		p1_sepVelocityWatch->visible = false;
		p2_sepVelocityWatch->visible = false;
	}

	DemoWindow::Update();
}

void CollisionPositionDemo::CheckForCollision()
{
	float p1_r = p1.Mass() / 3.0f;	// 3.0 from GridWidget_2D.cpp #define POINT_RADIUS_SCALE
	float p2_r = p2.Mass() / 3.0f;

	if (glm::length(p1.Position - p2.Position) <= (p1_r + p2_r))
	{
		contact.penetration = (p1_r + p2_r) - glm::length(p1.Position - p2.Position);
		Collide();
	}
}

void CollisionPositionDemo::Collide()
{
	p1.InverseMass = 1 / p1_mass;
	p2.InverseMass = 1 / p2_mass;

	glm::vec2 p1_prevPos = p1.Position;
	glm::vec2 p2_prevPos = p2.Position;

	if(contact.penetration == 0)
		contact.penetration = 0.25f;
	contact.resolveInterpenetration();

	p1_interpenDiff = p1.Position - p1_prevPos;
	p2_interpenDiff = p2.Position - p2_prevPos;

	p1.Position = p1_prevPos;
	p2.Position = p2_prevPos;
	
	//DemoWindow::Update();

	autostepWatchInfo->theCheckbox->setChecked(false);
	autostep = false;

	showVelocityVectors = false;
	p1_interpenDiffWatch->visible = true;
	p2_interpenDiffWatch->visible = true;
}

void CollisionPositionDemo::ResetParticles()
{
	// 45* angle
	//p1.Position = glm::vec2(-5,-5);
	//p2.Position = glm::vec2(5,5);
	//
	//p1.Velocity = glm::vec2(sqrtf(2) / 2.0f,sqrtf(2) / 2.0f);
	//p2.Velocity = -glm::vec2(p1.Velocity);

	// flat
	//p1.Position = glm::vec2(-5,0);
	//p2.Position = glm::vec2(5,0);
	//
	//p1.Velocity = glm::vec2(1,0);
	//p2.Velocity = glm::vec2(-1,0);//-glm::vec2(p1.Velocity);

	// some random angle
	p1.Position = glm::vec2(-3,-2);
	p2.Position = glm::vec2(3,5);
	
	p1.Velocity = glm::vec2(3, 2);
	p2.Velocity = glm::vec2(-1,-3);

	// intersect
	//p1.Position = glm::vec2(-100, 0);
	//p2.Position = glm::vec2(0, 100);
	//
	//p1.Velocity = glm::vec2(1, 0);
	//p2.Velocity = glm::vec2(0, -1);

	contactNormal = glm::normalize(p1.Position - p2.Position);

	p1SepVelocity = contactNormal * glm::dot(p1.Velocity, contactNormal);
	p2SepVelocity = contactNormal * glm::dot(p2.Velocity, contactNormal);

	contact.penetration = 0;
	p1_interpenDiff = glm::vec2(0,0);
	p2_interpenDiff = glm::vec2(0,0);

	showVelocityVectors = true;
	p1_interpenDiffWatch->visible = false;
	p2_interpenDiffWatch->visible = false;

	DemoWindow::Update();
}