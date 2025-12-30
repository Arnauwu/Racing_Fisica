#include "Globals.h"
#include "Application.h"
#include "Player.h"
#include "ModuleGame.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled, int x, int y) : Module(app, start_enabled), carX(x), carY(y)
{}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	physics = App->physics;

	LOG("Loading player");
	carText = LoadTexture("Assets/Characters/karts_spritesheet.png");
	myCar = new Car(App->physics, carX, carY, App->scene_intro, carText);
	myCar->type = EntityType::CAR;
	myCar->body->entity = myCar;
	myCar->App = App;
	myCar->body->ctype = ColliderType::CAR;
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
	if (App->scene_intro->currentScreen == Screens::GAME) {
		Input();
		if (actTime.ReadSec() > 0.25) {
			canAct = true;
			if (dashing) {
				dashing = false;
				myCar->maxVelocity = 3.6f;
			}
		}
		if (myCar->laps == 1) {
			myCar->laps = 0;
			App->scene_intro->currentScreen = Screens::END_RANK;
			App->scene_intro->UnloadGame();
			App->scene_intro->LoadScreen();
			DeleteMyCar();
		}
		else {
			physics->MouseJoint(myCar->body->body);
		}
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::Input() {
	TurnCar();
	if(IsKeyPressed(KEY_SPACE) && canAct)
	{
		Action();
	}
	if (IsKeyPressed(KEY_ONE)) {
		character = Characters::KNIGHT;
	}
	if (IsKeyPressed(KEY_TWO)) {
		character = Characters::HORNET;
	}
	if (IsKeyPressed(KEY_THREE)) {
		character = Characters::ZOTE;
	}
	if (IsKeyPressed(KEY_FOUR)) {
		character = Characters::SHERMA;
	}
	if (IsKeyPressed(KEY_FIVE)) {
		character = Characters::P_KING;
	}
	if (IsKeyPressed(KEY_SIX)) {
		character = Characters::PABLO;
	}
}

void ModulePlayer::Action() 
{
	b2Body* carBody = myCar->body->body;
	Dash();
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

void ModulePlayer::DeleteMyCar(){
	for (auto it = App->scene_intro->entities.begin(); it != App->scene_intro->entities.end();)//Delete all cars
	{
		if ((*it)->body != nullptr) {
			if ((*it) == myCar)
			{
				App->physics->DeleteBody((*it)->body);
				delete* it;
				it = App->scene_intro->entities.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}
