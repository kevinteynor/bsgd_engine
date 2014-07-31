#include "SpringDemo.h"
#include <Qt\qpainter.h>
#include <UpdateClock.h>
#include <VerticalDebugMenu.h>

void SpringDemo::InitializeDebugGui()
{
	ResetPoints();

	 // BASIC SPRING
	// VerticalDebugMenu::WatchFloatSlider("K", &springForceGenerator_p1.springConstant);
	// VerticalDebugMenu::WatchFloatSlider("Rest Length", &springForceGenerator_p1.restLength);
	// DemoWindow::WatchVector(&p1.Position, Qt::red);
	// DemoWindow::WatchVector(&p2.Position, Qt::green);
	// forceRegistry.Add(&p1, &springForceGenerator_p2);
	// forceRegistry.Add(&p2, &springForceGenerator_p1);
	//
	// // ANCHORED SPRING
	// VerticalDebugMenu::WatchFloatSlider("K", &anchoredSpringGenerator.springConstant);
	// DemoWindow::WatchVector(&p3.Position, Qt::yellow);
	// forceRegistry.Add(&p3, &anchoredSpringGenerator);
	//
	// // BUOYANCY
	// VerticalDebugMenu::WatchFloatSlider("Liquid Level", &buoyancyForceGenerator.liquidHeight);
	// DemoWindow::WatchVector(&p4.Position, Qt::blue);
	// forceRegistry.Add(&p4, &gravityForceGenerator);
	// forceRegistry.Add(&p4, &buoyancyForceGenerator);
	//
	// // SPRING-CHAIN
	// VerticalDebugMenu::WatchFloatSlider("Px", &p5.Position.x);
	// VerticalDebugMenu::WatchFloatSlider("Py", &p5.Position.y);
	// DemoWindow::WatchVector(&p5.Position, Qt::green);
	// DemoWindow::WatchVector(&p6.Position, Qt::green);
	// DemoWindow::WatchVector(&p7.Position, Qt::green);
	// DemoWindow::WatchVector(&p8.Position, Qt::green);
	// forceRegistry.Add(&p6, &p5_SpringForce);
	// forceRegistry.Add(&p7, &p6_SpringForce);
	// forceRegistry.Add(&p8, &p7_SpringForce);

	// // BUNGEE
	 VerticalDebugMenu::WatchFloatSlider("K", &bungeeForce_p9.springConstant);
	 VerticalDebugMenu::WatchFloatSlider("Rest Length", &bungeeForce_p9.restLength);
	 DemoWindow::WatchVector(&p9.Position, Qt::red);
	 DemoWindow::WatchVector(&p10.Position, Qt::green);
	 forceRegistry.Add(&p9, &bungeeForce_p10);
	 forceRegistry.Add(&p10, &bungeeForce_p9);

}

void SpringDemo::Update()
{
	springForceGenerator_p2.springConstant = springForceGenerator_p1.springConstant;
	springForceGenerator_p2.restLength = springForceGenerator_p1.restLength;
	bungeeForce_p10.springConstant = bungeeForce_p9.springConstant;
	bungeeForce_p10.restLength = bungeeForce_p9.restLength;

	DemoWindow::Update();
	forceRegistry.UpdateForces();

	p1.Update();
	p2.Update();
	p3.Update();
	p4.Update();
	p5.Update();
	p6.Update();
	p7.Update();
	p8.Update();
	p9.Update();
	p10.Update();
}

void SpringDemo::ResetPoints()
{
	p1.Position = glm::vec2(-1, 3);
	p2.Position = glm::vec2(1, 3);

	p3.Position = glm::vec2(0, -3);
	p3.Acceleration = glm::vec2(0, -10);
	anchorPoint = glm::vec2(0, 0);

	p4.Position = glm::vec2(0, -2);

	p5.Position = glm::vec2(0, 0);
	p6.Position = glm::vec2(0, -3);
	p7.Position = glm::vec2(0, -6);
	p8.Position = glm::vec2(0, -9);

	p9.Position = glm::vec2(-5, 0);
	p10.Position = glm::vec2(5, 0);
}