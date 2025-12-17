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
	carText = LoadTexture("Assets/Characters/Hornet.png");
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

update_status ModulePlayer::Update()
{
	Input();
	return UPDATE_CONTINUE;
}

void ModulePlayer::Input() {
	b2Body* carBody = myCar->body->body;
	if (IsKeyDown(KEY_A))
	{
		if (carBody->GetAngularVelocity() >= -0.5f) {
			carBody->SetAngularVelocity(carBody->GetAngularVelocity() - 0.5f);
		}
	}
	else if(!IsKeyDown(KEY_D)){
		carBody->SetAngularVelocity(0.0f);
	}

	if (IsKeyDown(KEY_D))
	{
		if (carBody->GetAngularVelocity() <= 0.5f) {
			carBody->SetAngularVelocity(carBody->GetAngularVelocity() + 0.5f);
		}
	}
	else if (!IsKeyDown(KEY_A)) {
		carBody->SetAngularVelocity(0.0f);
	}

	if(IsKeyDown(KEY_SPACE))
	{
		carBody->SetLinearVelocity({ 0,0 });
	}
}

