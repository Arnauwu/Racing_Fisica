#pragma once

#include "Globals.h"
#include "Module.h"
#include "ModulePhysics.h"
#include "Entity.h"

#include "p2Point.h"

#include "raylib.h"

class Car : public Entity
{
public:
	Car();
	Car(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Entity(physics->CreateRectangle(_x, _y, 32, 32), _listener), texture(_texture){}

	void Update() override;

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override;

	void Move();

	Application* App;
	double carRotation = 0;
	float maxVelocity = 1.8f;
	b2Vec2 impulse = { 0.0f, -maxVelocity };
private:
	Texture2D texture;
	int textTiles;
};