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
	return UPDATE_CONTINUE;
}

void ModulePlayer::Input() {
	b2Body* carBody = myCar->body->body;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		myCar->carRotation -= 2.5f;
		if (myCar->carRotation < 360 && myCar->carRotation > 270) {//Direction Left - Up case
			myCar->impulse.x -= 0.1;
			myCar->impulse.y += 0.1;
		}
		else if (myCar->carRotation < 270 && myCar->carRotation > 180) {//Direction Left - Down case
			myCar->impulse.x += 0.1;
			myCar->impulse.y += 0.1;

		}
		else if (myCar->carRotation < 180 && myCar->carRotation > 90) {//Direction Right - Down case
			myCar->impulse.x += 0.1;
			myCar->impulse.y -= 0.1;
		}
		else {//Direction Right - Up case
			myCar->impulse.x -= 0.1;
			myCar->impulse.y -= 0.1;
		}
	}
	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		myCar->carRotation += 2.5f;
		if (myCar->carRotation < 360 && myCar->carRotation > 270) {//Direction Left - Up case
			myCar->impulse.x += 0.1;
			myCar->impulse.y -= 0.1;
		}
		else if (myCar->carRotation < 270 && myCar->carRotation > 180) {//Direction Left - Down case
			myCar->impulse.x -= 0.1;
			myCar->impulse.y -= 0.1;
		}
		else if (myCar->carRotation < 180 && myCar->carRotation > 90) {//Direction Left - Down case
			myCar->impulse.x -= 0.1;
			myCar->impulse.y += 0.1;
		}
		else {//Direction Right - Down case
			myCar->impulse.x += 0.1;
			myCar->impulse.y += 0.1;
		}
	}
	if(IsKeyPressed(KEY_SPACE))
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
		break;
	case HORNET:
		if (carBody->GetType() != b2_staticBody) {
			carBody->SetType(b2_staticBody);
		}
		else {
			carBody->SetType(b2_dynamicBody);
		}
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