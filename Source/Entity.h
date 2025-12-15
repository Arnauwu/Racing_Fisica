#pragma once

#include "Globals.h"
#include "Module.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include <set>
class PhysBody;
class Entity
{
protected:

	Entity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
	}

public:
	virtual ~Entity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

public:
	PhysBody* body;
	Module* listener;
};