#include "Globals.h"
#include "Application.h"
#include "Player.h"
#include "ModuleGame.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	carText = LoadTexture("Assets/crate.png");
	myCar = new Car(App->physics, 400, 400, this, carText);
	App->scene_intro->entities.emplace_back(myCar);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}



