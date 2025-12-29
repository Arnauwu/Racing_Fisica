#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"
#include <vector>

#define GRAVITY_X 0.0f
#define GRAVITY_Y 0.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class Entity;
enum class ColliderType {
	TURBO,
	CHECKPOINT,
	TURN_LEFT,
	TURN_RIGHT,
	CAR
};
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{
	}

	//void GetPosition(int& x, int& y) const;
	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	ColliderType ctype;
	Entity* entity;
	int identifier;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateCircle(int x, int y, int radius, b2Vec2 initialVelocity, float mass);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, const int* points, int size);
	void DeleteBody(PhysBody* body);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	b2World* world;

	void MouseJoint(b2Body* targetBody);
	b2Body* groundBody = nullptr;
	b2MouseJoint* mouseJoint = nullptr;

private:

	bool debug;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};