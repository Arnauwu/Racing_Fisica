#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Car.h"
#include "Timer.h"
#include "Application.h"

class Enemy : public Entity
{
public:
	Enemy();

	void Start();
	void Update() override;
	void TurnCar();
	void DeleteMyCar();
	bool canAct = true;
	bool dashing = false;


	Car* myCar;
	Texture2D carText;
	int carX, carY;

	Characters character = KNIGHT;

	Application* App;
};