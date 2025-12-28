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
	Car(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Entity(physics->CreateRectangle(_x, _y, 16, 16), _listener), texture(_texture){}

	void Update() override;

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override;

	void Move();

	void CheckFinishLane();

	void Jump();
	bool jumping = false;

	Application* App;
	double carRotation = 180;
	float scale = 1;
	float maxVelocity = 3.6f;
	b2Vec2 impulse = { 0.0f, -maxVelocity };
	int laps = 0;
	Characters* character;
	std::vector<PhysBody*> checkPoints;
	Rectangle guideDown = { 0,0,25,25 };
	Vector2 rectOffset = { -12, 60 };
private:
	Texture2D texture;
	int textTiles;
	int frameCount = 0;
	int drawFrame = 0;
	bool landing = false;
};