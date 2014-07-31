#include "MultipleParticlesDemo.h"
#include <DemoWindow.h>
#include <glm\glm.hpp>
#include <GridWidget_2D.h>
#include <ParticleContact_2D.h>
#include <ParticleForceGenerator_2D.h>
#include <PhysicsParticle_2D.h>
#include <VerticalDebugMenu.h>
#include <UpdateClock.h>

#include <iostream>

void MultipleParticlesDemo::InitializeDebugGui()
{
	autostep = false;
	fps = 240;

	ResetParticles();

	p_groundFriction.y = 0;
	forceRegistry.Add(&p, &p_groundFriction);

	b_groundFriction.y = 0;
	for(uint i = 0; i < NUM_BOXES; i++)
	{
		forceRegistry.Add(&boxes[i], &b_groundFriction);
	}

	VerticalDebugMenu::WatchBool("Autostep", &autostep);
	Delegate f;
	f.bind(this, &MultipleParticlesDemo::ResetParticles);
	VerticalDebugMenu::WatchButton("Reset", f);
	
	DemoWindow::WatchPoint(&p.Position, &p.InverseMass, Qt::red, true);
	for(uint i = 0; i < NUM_BOXES; i++)
	{
		DemoWindow::WatchPoint(&boxes[i].Position, &boxes[i].InverseMass, Qt::green, true);
	}
}

void MultipleParticlesDemo::Step()
{
	UpdateClock::SetDeltaTime(1 / fps);

	p.Update();
	for(uint i = 0; i < NUM_BOXES; i++)
	{
		boxes[i].Update();
	}

	std::cout << p.Position.y << std::endl;

	CheckForCollision();

	forceRegistry.UpdateForces();
}

void MultipleParticlesDemo::Update()
{
	if(autostep) Step();

	DemoWindow::Update();
}

void MultipleParticlesDemo::CheckForCollision()
{
	// check all particles against eachother and the ground.
	// RADIUS = Mass * 3

	float r_p = p.Mass() / 3;

	for(uint i = 0; i < NUM_BOXES; i++)
	{
		float r_i = boxes[i].Mass() / 3;

		// Check against ground
		if(boxes[i].Position.y <= 0)
		{
			ParticleContact_2D contact = ParticleContact_2D(&boxes[i], NULL, 0.5f);
			contact.contactNormal = glm::vec2(0, 1);
			contact.penetration = -(boxes[i].Position.y);
			contact.Resolve();
		}

		// Check against particle
		if(glm::length(boxes[i].Position - p.Position) < r_i + r_p)
		{
			// collision
			ParticleContact_2D contact = ParticleContact_2D(&boxes[i], &p, 0.5f);
			contact.penetration = -(glm::length(boxes[i].Position - p.Position) - (r_i + r_p));
			contact.Resolve();
		}
			
		// Check against other boxes
		for(uint j = i + 1; j < NUM_BOXES; j++)
		{
			float r_j = boxes[j].Mass() / 3;

			if(glm::length(boxes[i].Position - boxes[j].Position) < r_i + r_j )
			{
				// Collision!
				ParticleContact_2D contact = ParticleContact_2D(&boxes[i], &boxes[j], 0.35f);
				contact.penetration = -(glm::length(boxes[i].Position - boxes[j].Position) - (r_i + r_j));
				contact.Resolve();
			}
		}
	}

	if(p.Position.y <= 0)
	{
		ParticleContact_2D contact = ParticleContact_2D(&p, NULL, 0.5f);
		contact.contactNormal = glm::vec2(0,1);
		contact.penetration = -(p.Position.y);
		contact.Resolve();
	}
}

void MultipleParticlesDemo::Collide()
{

}

void MultipleParticlesDemo::ResetParticles()
{
	p.Position = glm::vec2(-10, 4);
	p.Velocity = glm::vec2(0,0);
	p.Acceleration = glm::vec2(0,-10);
	p.InverseMass = 1.0f / 5;
	
	float x = ((float)rand() / RAND_MAX * 10) + 10;
	float y = ((float)rand() / RAND_MAX * 5) + 2;

	p.AddImpulse(glm::vec2(x, y));

	for(uint i = 0; i < NUM_BOXES; i++)
	{
		float px = (int)((float)rand() / RAND_MAX * 3);
		boxes[i].Update();
		boxes[i].InverseMass = 1.0f / (((float)rand() / RAND_MAX * 4) + 1);
		boxes[i].Position = glm::vec2(px * 3, (boxes[i].Mass() / 3.0f) + ((boxes[i].Mass() / 1.4f) * i));
		boxes[i].Velocity = glm::vec2(0, 0);
		boxes[i].Acceleration = glm::vec2(0, -10);
	}
}