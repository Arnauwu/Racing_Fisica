
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
	currentScreen = Screens::MAIN_MENU;
	player = new ModulePlayer(App, true, 2000, 2000);
	enemy1 = new Enemy();
	enemy2 = new Enemy();
	enemy3 = new Enemy();
	enemy4 = new Enemy();
	enemy5 = new Enemy();
	player->App = app;
	/*player->carText = LoadTexture("Assets/Characters/karts_spritesheet.png");
	player->myCar = new Car(App->physics, 2000, 2000, App->scene_intro, player->carText);
	player->myCar->body->entity = player->myCar;
	player->myCar->App = App;
	player->myCar->body->ctype = ColliderType::CAR;
	player->myCar->body->body->SetFixedRotation(true);
	player->myCar->character = &player->character;
	App->scene_intro->entities.emplace_back(player->myCar);*/
	
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	enemies.push_back(enemy1);
	enemies.push_back(enemy2);
	enemies.push_back(enemy3);
	enemies.push_back(enemy4);
	enemies.push_back(enemy5);
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->App = App;
		enemies[i]->Start();
	}
	App->renderer->camera.zoom = 1.0f;
	App->renderer->camera.target = { 0.0f, 0.0f };
	App->renderer->camera.offset = { 0.0f, 0.0f };
	App->renderer->camera.rotation = 0.0f;

	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");
	currentScreen = Screens::MAIN_MENU;
	LoadScreen();

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	float currentZoom, halfScreenWidth, halfScreenHeight, visibleHalfWidth, visibleHalfHeight, targetX, targetY;

	switch (currentScreen) {
	case Screens::MAIN_MENU:
		if (IsKeyPressed(KEY_ENTER)) {
			currentScreen = Screens::CHAR_SELECT;
			LoadScreen();
		}
		break;
	case Screens::CHAR_SELECT:
		if (IsKeyPressed(KEY_ENTER)) {
			currentScreen = Screens::MAP_SELECT;
			LoadScreen();
		}
		break;
	case Screens::MAP_SELECT:
		if (IsKeyPressed(KEY_ONE)) {
			LoadMap(Maps::MOSS_GROTTO_1);
		}
		if (IsKeyPressed(KEY_TWO)) {
			LoadMap(Maps::CRYSTAL_PEAK_1);
		}
		break;
	case Screens::GAME:
		int playerX, playerY;
		player->myCar->body->GetPhysicPosition(playerX, playerY);

		halfScreenWidth = SCREEN_WIDTH / 2.0f;
		halfScreenHeight = SCREEN_HEIGHT / 2.0f;

		currentZoom = 1.5f;

		visibleHalfWidth = halfScreenWidth / currentZoom;
		visibleHalfHeight = halfScreenHeight / currentZoom;

		targetX = (float)playerX;
		targetY = (float)playerY;

		SetCamera(currentZoom, Vector2{ halfScreenWidth, halfScreenHeight }, Vector2{ targetX, targetY });
		break;
	case Screens::END_RANK:
		SetCamera(1.0f, Vector2{0,0}, Vector2{ 0,0 });
		if (IsKeyPressed(KEY_ENTER)) {
			currentScreen = Screens::MAIN_MENU;
			LoadScreen();
		}
		break;
	}


	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	for (Entity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}


	// ray -----------------
	if (ray_on == true)
	{
		vec2f destination((float)(mouse.x - ray.x), (float)(mouse.y - ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}

	for (const auto& collision : collidingEntities)
	{
		float totalMass = 0.0f;
		float sumRadius = 0.0f;

		b2Vec2 momentum;
		b2Vec2 position;
		momentum.x = 0.0f;
		momentum.y = 0.0f;
		for (Entity* entity : collision)
		{
			const b2Fixture* fixture = entity->body->body->GetFixtureList();
			const b2CircleShape* circle = dynamic_cast<const b2CircleShape*>(fixture->GetShape());
			sumRadius += circle->m_radius;
			entities.erase(std::find(entities.begin(), entities.end(), entity));
			b2Vec2 vec = entity->body->body->GetLinearVelocity();
			momentum.x += vec.x * entity->body->body->GetMass();
			momentum.y += vec.y * entity->body->body->GetMass();
			totalMass += entity->body->body->GetMass();
			float x = 0;
			float y = 0;
			position += entity->body->body->GetPosition();
			App->physics->DeleteBody(entity->body);
		}

		b2Vec2 velocity;
		velocity.x = momentum.x / totalMass;
		velocity.y = momentum.y / totalMass;
		position.x /= 2;
		position.y /= 2;
	}

	collidingEntities.clear();

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	Car* car = (Car*)bodyA->entity;
	switch (bodyB->ctype)
	{
	case ColliderType::CHECKPOINT:
		CalculatePositions();
		printf("\n\n");
		for (int i = 0; i < cars.size(); i++) {
			printf("%d\n", (int)*cars[i]->character);
		}
		if (car->checkPoints.size() < 4) {
			bool isThere = false;
			for (int i = 0; i < car->checkPoints.size(); i++) {
				if (car->checkPoints.at(i) == bodyB) isThere = true;
			}
			if (!isThere) car->checkPoints.push_back(bodyB);
		}
		else if (bodyB->identifier == 1){
			car->checkPoints.clear();
			car->checkPoints.push_back(bodyB);
			car->laps++;
		}
		break;
	case ColliderType::TURBO:
		break;
	default:
		break;
	}
}

void ModuleGame::LoadMap(Maps _map) {

	switch (_map) {
	case Maps::MOSS_GROTTO_1:
		currentScreen = Screens::GAME;
		App->renderer->backgroundTexture = LoadTexture("Assets/Maps/MossGrotto.png");
		EXTERIOR = App->physics->CreateChain(0, 0, MossGrottoEXT, 196);
		EXTERIOR->body->SetType(b2BodyType::b2_staticBody);
		INTERIOR = App->physics->CreateChain(0, 0, MossGrottoINT, 156);
		INTERIOR->body->SetType(b2BodyType::b2_staticBody);
		CheckPoint1 = App->physics->CreateRectangleSensor(155, 600, 210, 30);
		CheckPoint1->identifier = 1;
		CheckPoint2 = App->physics->CreateRectangleSensor(1140, 850, 300, 30);
		CheckPoint3 = App->physics->CreateRectangleSensor(725, 360, 150, 30);
		CheckPoint4 = App->physics->CreateRectangleSensor(525, 350, 210, 30);
		CheckPoint1->ctype = ColliderType::CHECKPOINT;
		CheckPoint2->ctype = ColliderType::CHECKPOINT;
		CheckPoint3->ctype = ColliderType::CHECKPOINT;
		CheckPoint4->ctype = ColliderType::CHECKPOINT;

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(175, 1005, 250, 250));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1140, 970, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1140, 100, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(720, 200, 200, 400));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(700, 800, 200, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(390, 800, 100, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(400, 550, 100, 100));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(525, 500, 100, 100));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(525, 150, 200, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(200, 150, 200, 200));

			enemies[i]->turnLeft[0]->identifier = 1;
			enemies[i]->turnLeft[1]->identifier = 0;
			enemies[i]->turnLeft[2]->identifier = 3;
			enemies[i]->turnLeft[3]->identifier = 2;
			enemies[i]->turnRight[0]->identifier = 3;
			enemies[i]->turnRight[1]->identifier = 0;
			enemies[i]->turnRight[2]->identifier = 1;
			enemies[i]->turnLeft[4]->identifier = 0;
			enemies[i]->turnLeft[5]->identifier = 3;
			enemies[i]->turnLeft[6]->identifier = 2;
		}
		
		break;

	case Maps::MOSS_GROTTO_2:
		currentScreen = Screens::GAME;
		break;
	case Maps::CRYSTAL_PEAK_1:
		currentScreen = Screens::GAME;
		App->renderer->backgroundTexture = LoadTexture("Assets/Maps/CrystalPeak.png");
		EXTERIOR = App->physics->CreateChain(0, 0, CrystalPeakEXT, 174);
		EXTERIOR->body->SetType(b2BodyType::b2_staticBody);
		INTERIOR = App->physics->CreateChain(0, 0, CrystalPeakINT, 168);
		INTERIOR->body->SetType(b2BodyType::b2_staticBody);
		CheckPoint1 = App->physics->CreateRectangleSensor(100, 700, 210, 30);
		CheckPoint1->identifier = 1;
		CheckPoint2 = App->physics->CreateRectangleSensor(425, 500, 210, 30);
		CheckPoint3 = App->physics->CreateRectangleSensor(500, 75, 30, 150);
		CheckPoint4 = App->physics->CreateRectangleSensor(1175, 700, 250, 30);
		CheckPoint1->ctype = ColliderType::CHECKPOINT;
		CheckPoint2->ctype = ColliderType::CHECKPOINT;
		CheckPoint3->ctype = ColliderType::CHECKPOINT;
		CheckPoint4->ctype = ColliderType::CHECKPOINT;

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(115, 1180, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(435, 1180, 250, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(465, 305, 250, 220));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(935, 300, 125, 300));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(935, 600, 250, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(935, 600, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(635, 600, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(615, 820, 210, 130));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(885, 800, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(855, 1000, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1255, 1000, 250, 200));

			enemies[i]->turnLeft[0]->identifier = 1; //DRETA
			enemies[i]->turnLeft[1]->identifier = 0; //ADALT
			enemies[i]->turnRight[0]->identifier = 1;
			enemies[i]->turnRight[1]->identifier = 2; //AVALL
			enemies[i]->turnRight[2]->identifier = 3; //ESQUERRA
			enemies[i]->turnRight[3]->identifier = 2;
			enemies[i]->turnLeft[2]->identifier = 1;
			enemies[i]->turnLeft[3]->identifier = 2;
			enemies[i]->turnRight[4]->identifier = 1;
			enemies[i]->turnLeft[4]->identifier = 0;
			enemies[i]->turnLeft[5]->identifier = 1;


		}
		break;
	case Maps::CRYSTAL_PEAK_2:
		currentScreen = Screens::GAME;
		break;
	case Maps::BELLHART_1:
		currentScreen = Screens::GAME;
		break;
	case Maps::BELLHART_2:
		currentScreen = Screens::GAME;
		break;
	}
	SetUpCars();
}

void ModuleGame::carSetup(Car* _car, Characters* _char) {
	_car->body->entity = _car;
	_car->App = App;
	_car->body->ctype = ColliderType::CAR;
	_car->body->body->SetFixedRotation(true);
	_car->character = _char;
	App->scene_intro->entities.emplace_back(_car);
	_car->timer.Start();
}

void ModuleGame::LoadScreen() {
	switch (currentScreen) {
	case Screens::MAIN_MENU:
		App->renderer->backgroundTexture = LoadTexture("Assets/Placeholders/Main_Menu.png");
		break;
	case Screens::CHAR_SELECT:
		App->renderer->backgroundTexture = LoadTexture("Assets/Placeholders/Character_Select.png");
		break;
	case Screens::MAP_SELECT:
		App->renderer->backgroundTexture = LoadTexture("Assets/Placeholders/Map_Select.png");
		break;
	case Screens::GAME:
		LoadMap(map); // !!!!!!!!!!!!!!
		break;
	case Screens::END_RANK:
		App->renderer->backgroundTexture = LoadTexture("Assets/Placeholders/End_Ranking.png");
		break;
	}
}

void ModuleGame::UnloadGame() {
	App->physics->DeleteBody(EXTERIOR);
	App->physics->DeleteBody(INTERIOR);
	App->physics->DeleteBody(CheckPoint1);
	App->physics->DeleteBody(CheckPoint2);
	App->physics->DeleteBody(CheckPoint3);
	App->physics->DeleteBody(CheckPoint4);
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->DeleteMyCar();
		for (int j = 0; j < enemies[i]->turnRight.size(); j++) {
			App->physics->DeleteBody(enemies[i]->turnRight[j]);
		}
		for (int j = 0; j < enemies[i]->turnLeft.size(); j++) {
			App->physics->DeleteBody(enemies[i]->turnLeft[j]);
		}
		enemies[i]->turnLeft.clear();
		enemies[i]->turnRight.clear();
	}
}

void ModuleGame::SetCamera(float zoom, Vector2 offset, Vector2 target) {
	App->renderer->camera.zoom = zoom;
	App->renderer->camera.offset = offset;
	App->renderer->camera.target = target;
}

void ModuleGame::SetUpCars() {
	player->myCar = new Car(App->physics, 100, 600, App->scene_intro, player->carText);
	carSetup(player->myCar, &player->character);
	cars.push_back(player->myCar);
	enemy1->character = KNIGHT;
	enemy2->character = ZOTE;
	enemy3->character = SHERMA;
	enemy4->character = P_KING;
	enemy5->character = PABLO;
	enemy1->myCar = new Car(App->physics, 140, 575, App->scene_intro, enemy1->carText);
	enemy2->myCar = new Car(App->physics, 180, 550, App->scene_intro, enemy2->carText);
	enemy3->myCar = new Car(App->physics, 100, 525, App->scene_intro, enemy3->carText);
	enemy4->myCar = new Car(App->physics, 140, 500, App->scene_intro, enemy4->carText);
	enemy5->myCar = new Car(App->physics, 180, 475, App->scene_intro, enemy5->carText);
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->turnTargetRotation = 180;
		if (enemies[i]->character == player->character) {
			enemies[i]->character = HORNET;
		}
		carSetup(enemies[i]->myCar, &enemies[i]->character);
		cars.push_back(enemies[i]->myCar);
	}
}

void ModuleGame::CalculatePositions() {
	int n = cars.size();

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {

			// 1. Voltes (més és millor)
			if (cars[j]->laps < cars[j + 1]->laps) {
				std::swap(cars[j], cars[j + 1]);
			}
			// 2. Checkpoints (més és millor)
			else if (cars[j]->laps == cars[j + 1]->laps &&
				cars[j]->checkPoints.size() < cars[j + 1]->checkPoints.size()) {
				std::swap(cars[j], cars[j + 1]);
			}
		}
	}
}