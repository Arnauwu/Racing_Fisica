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
	myCar->App = App;
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
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		myCar->carRotation -= 2.5f;
		if (myCar->carRotation < 360 && myCar->carRotation > 270) {//Direction Left - Up case
			myCar->impulse.x -= 0.05;
			myCar->impulse.y += 0.05;
		}
		else if (myCar->carRotation < 270 && myCar->carRotation > 180) {//Direction Left - Down case
			myCar->impulse.x += 0.05;
			myCar->impulse.y += 0.05;
		}
		else if (myCar->carRotation < 180 && myCar->carRotation > 90) {//Direction Right - Down case
			myCar->impulse.x += 0.05;
			myCar->impulse.y -= 0.05;
		}
		else {//Direction Right - Up case
			myCar->impulse.x -= 0.05;
			myCar->impulse.y -= 0.05;
		}
	}
	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		myCar->carRotation += 2.5f;
		if (myCar->carRotation < 360 && myCar->carRotation > 270) {//Direction Left - Up case
			myCar->impulse.x += 0.05;
			myCar->impulse.y -= 0.05;
		}
		else if (myCar->carRotation < 270 && myCar->carRotation > 180) {//Direction Left - Down case
			myCar->impulse.x -= 0.05;
			myCar->impulse.y -= 0.05;
		}
		else if (myCar->carRotation < 180 && myCar->carRotation > 90) {//Direction Left - Down case
			myCar->impulse.x -= 0.05;
			myCar->impulse.y += 0.05;
		}
		else {//Direction Right - Down case
			myCar->impulse.x += 0.05;
			myCar->impulse.y += 0.05;
		}
	}
	if(IsKeyDown(KEY_SPACE))
	{
		carBody->SetLinearVelocity({ 0,0 });
	}
}

