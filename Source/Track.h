#pragma once
#include "Globals.h"
#include "Module.h"
#include "ModulePhysics.h"
#include "Entity.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include <set>

class Track : public Entity
{
public:
	Track(ModulePhysics* physics, int _x, int _y, const int _EXT[], int _size, const int _INT[], Module* _listener, Texture2D _texture) : Entity(physics->CreateChain(_x, _y, _EXT, _size), _listener), INT(_INT), texture(_texture) {}
	Texture2D texture;
	void Update();
	const int* INT;
};