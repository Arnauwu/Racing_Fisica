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
	carText = LoadTexture("Assets/Characters/karts_spritesheet.png");
	myCar = new Car(App->physics, 490, 400, this, carText);
	myCar->App = App;
	myCar->body->body->SetFixedRotation(true);
	myCar->character = &character;
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
	if (actTime.ReadSec() > 0.25) {
		canAct = true;
		if (dashing) {
			dashing = false;
			myCar->maxVelocity -= 3;
		}
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::Input() {
	b2Body* carBody = myCar->body->body;
	TurnCar();
	if(IsKeyPressed(KEY_SPACE) && canAct)
	{
		Action();
	}
	if (IsKeyPressed(KEY_ONE)) {
		character = KNIGHT;
	}
	if (IsKeyPressed(KEY_TWO)) {
		character = HORNET;
	}
	if (IsKeyPressed(KEY_THREE)) {
		character = ZOTE;
	}
	if (IsKeyPressed(KEY_FOUR)) {
		character = SHERMA;
	}
	if (IsKeyPressed(KEY_FIVE)) {
		character = P_KING;
	}
	if (IsKeyPressed(KEY_SIX)) {
		character = PABLO;
	}
}

void ModulePlayer::Action() 
{
	b2Body* carBody = myCar->body->body;
	switch (character)
	{
	case KNIGHT:
		myCar->jumping = true;
		break;
	case HORNET:
		Dash();
		break;
	case ZOTE:
		break;
	case SHERMA:
		break;
	case P_KING:
		break;
	case PABLO:
		break;
	}
}

void ModulePlayer::Dash() {
	myCar->maxVelocity += 3;
	canAct = false;
	dashing = true;
	actTime.Start();
}

void ModulePlayer::TurnCar() {
	if (!dashing) {
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			myCar->carRotation -= 2.5f;
		}
		else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			myCar->carRotation += 2.5f;
		}
	}

	// Adjust velocity to the direction the car is facing
	float angleRad = myCar->carRotation * (PI / 180.0f);

	myCar->impulse.x = myCar->maxVelocity * sin(angleRad);
	myCar->impulse.y = myCar->maxVelocity * -cos(angleRad);
}