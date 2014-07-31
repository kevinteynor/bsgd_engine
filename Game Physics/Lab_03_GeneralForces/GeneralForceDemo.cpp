#include "GeneralForceDemo.h"
#include <UpdateClock.h>
#include <VerticalDebugMenu.h>

void GeneralForceDemo::InitializeDebugGui()
{
	VerticalDebugMenu::WatchFloatSlider("K1", &dragForceGenerator.k1, 0, 10);
	VerticalDebugMenu::WatchFloatSlider("K2", &dragForceGenerator.k2, 0, 10);
	VerticalDebugMenu::WatchVec2("p1 position:", &p1.Position);
	VerticalDebugMenu::WatchVec2("p1 velocity:", &p1.Velocity);
	DemoWindow::WatchVector(&p1.Position, Qt::red);

	forceRegistry.Add(&p1, &dragForceGenerator);

	ResetParticle();
}

void GeneralForceDemo::Update()
{
	DemoWindow::Update();
	forceRegistry.UpdateForces();
	p1.Update();

	if(glm::length(p1.Velocity) < 0.1f)
		ResetParticle();
}

void GeneralForceDemo::ResetParticle()
{
	p1.Position = glm::vec2(0,0);
	p1.Velocity = glm::vec2(10,0);
}