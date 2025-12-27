#include "Globals.h"
#include "Application.h"
#include "Enemy.h"
#include "ModuleGame.h"

Enemy::Enemy()
	: Entity() {  // Llamada al constructor por defecto de Entity
	// Inicialización específica de Enemy
}

// Load assets
void Enemy::Start()
{
	LOG("Loading enemy");
	type = EntityType::ENEMY;
	carText = LoadTexture("Assets/Characters/karts_spritesheet.png");
	myCar = new Car(App->physics, carX, carY, App->scene_intro, carText);
	myCar->type = EntityType::CAR;
	myCar->body->entity = myCar;
	myCar->App = App;
	myCar->body->ctype = ColliderType::CAR;
	myCar->body->body->SetFixedRotation(true);
	myCar->character = &character;
	App->scene_intro->entities.emplace_back(this);
	App->scene_intro->entities.emplace_back(myCar);
}

void Enemy::Update()
{
	if (App->scene_intro->currentScreen == Screens::GAME) {
		TurnCar();
	}
}

void Enemy::TurnCar() {
	/*if (!dashing) {
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			myCar->carRotation -= 2.5f;
		}
		else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			myCar->carRotation += 2.5f;
		}
	}*/

	// Adjust velocity to the direction the car is facing
	float angleRad = myCar->carRotation * (PI / 180.0f);

	myCar->impulse.x = myCar->maxVelocity * sin(angleRad);
	myCar->impulse.y = myCar->maxVelocity * -cos(angleRad);
}

void Enemy::DeleteMyCar() {
	for (auto it = App->scene_intro->entities.begin(); it != App->scene_intro->entities.end();)
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