#include "ClothDemo.h"
#include <DemoWindow.h>
#include <GridWidget2D.h>
#include <VerticalDebugMenu.h>
#include <UpdateClock.h>

#include <iostream>

void ClothDemo::InitializeDebugGui()
{
	stiffness = 1;
	restlength = 3;
	tearlength = 10;
	frameSolvesPerFrame = 1;

	this->gridDisplay->ClickAndDrag = true;

	VerticalDebugMenu::WatchFloatSlider("Rest Length", &restlength, 0.5f, 10.0f);
	VerticalDebugMenu::WatchFloatSlider("Stiffness", &stiffness, 0.0f, 1.25f);
	VerticalDebugMenu::WatchFloatSlider("Solves/Frame", &frameSolvesPerFrame, 1, 4);

	Delegate f = Delegate();
	f.bind(this, &ClothDemo::ResetParticles);
	VerticalDebugMenu::WatchButton("Reset", f);

	
	for(uint r = 0; r < DIMENSION; r++)
	{
		for(uint c = 0; c < DIMENSION; c++)
		{
			points[r][c] = ClothP();

			if(r > 0) links_up[r][c] = ClothLink(&points[r - 1][c], &points[r][c], &restlength, &tearlength, &stiffness);
			if(c > 0) links_left[r][c] = ClothLink(&points[r][c - 1], &points[r][c], &restlength, &tearlength, &stiffness);

			DemoWindow::WatchPoint(&points[r][c].position, &points[r][c].imass, Qt::red);
		}
	}

	ResetParticles();
}

void ClothDemo::Step()
{
	for(int s = 0; s < (int)frameSolvesPerFrame; s++)
	{
		for(uint r = 0; r < DIMENSION; r++)
		{
			for(uint c = 0; c < DIMENSION; c++)
			{
				links_up[r][c].Solve();
				links_left[r][c].Solve();
			}
		}
	}
	for(uint r = 0; r < DIMENSION; r++)
	{
		for(uint c = 0; c < DIMENSION; c++)
		{
			points[r][c].Update(UpdateClock::DeltaTime());
		}
	}
	
	for(uint c = 0; c < DIMENSION; c++)
	{
		points[0][c].position = glm::vec2((c * restlength), 0);
	} 
}

void ClothDemo::Update()
{
	Step();

	DemoWindow::Update();
}

void ClothDemo::ResetParticles()
{
	for(uint r = 0; r < DIMENSION; r++)
	{
		for(uint c = 0; c < DIMENSION; c++)
		{
			points[r][c].position = glm::vec2((c * 2.5f), -(r * 2.5f));
			points[r][c].lastPosition = glm::vec2((c * 2.5f), -(r * 2.5f));
			points[r][c].acceleration = glm::vec2(0, -5);
		}
	}
}