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

	float carRotation = 0;
	float car_x = 0.0f;
	float car_y = 0.1f;
	b2Vec2 impulse = { 0.0f, -0.1f };
private:
	Texture2D texture;
	int textTiles;
	b2Vec2 limitV = { 1.0f, -1.0f };
	b2Vec2 velocity;
	float direction;
	//b2Vec2 impulseA = { -0.5f, 0.0f };
	//b2Vec2 impulseD = { 0.5f, 0.0f };
};