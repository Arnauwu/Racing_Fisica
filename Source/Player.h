#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Car.h"
#include "Timer.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void Input();
	void Action();
	void Dash();
	void TurnCar();
	bool canAct = true;
	bool dashing = false;


	Car* myCar;
	Texture2D carText;
	Timer actTime;

	Characters character = HORNET;
};