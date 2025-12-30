
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
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	wormTEXT = LoadTexture("Assets/Characters/Worm.png");
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

	audio = App->audio;

	moveMenuFx = audio->LoadFx("Assets/Audio/Fx/SelectPj.wav") - 1;
	optSelectFx = audio->LoadFx("Assets/Audio/Fx/salt.wav") - 1;
	winFX = audio->LoadFx("Assets/Audio/Fx/win.wav") - 1;
	looseFX = audio->LoadFx("Assets/Audio/Fx/loose.wav") - 1;
	turboFx = audio->LoadFx("Assets/Audio/Fx/turbo.wav") - 1;
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
			if (selected == 1) {
				audio->PlayFx(optSelectFx);
				currentScreen = Screens::CHAR_SELECT;
				LoadScreen();
			}
			else {
				exit(0);
			}
		}
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_W)) {
			if (selected == 1) {
				selected = 2;
			}
			else {
				selected = 1;
			}
			audio->PlayFx(moveMenuFx);
		}
		break;
	case Screens::CHAR_SELECT: //edit
		if (IsKeyPressed(KEY_ENTER)) {
			player->character = (Characters)(selected - 1);
			audio->PlayFx(optSelectFx);
			currentScreen = Screens::MAP_SELECT;
			LoadScreen();
		}
		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
			if (selected == 1) {
				selected = 6;
			}
			else {
				selected--;
			}
			audio->PlayFx(moveMenuFx);
		}
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
			if (selected == 6) {
				selected = 1;
			}
			else {
				selected++;
			}
			audio->PlayFx(moveMenuFx);
		}
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
			for (int i = 0; i < 3; i++) {
				if (selected - 1 < 1) {
					selected = 6;
				}
				else {
					selected--;
				}
			}
			audio->PlayFx(moveMenuFx);
		}
		else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
			for (int i = 0; i < 3; i++) {
				if (selected + 1 > 6) {
					selected = 1;
				}
				else {
					selected++;
				}
			}
			audio->PlayFx(moveMenuFx);
		}
		break;
	case Screens::MAP_SELECT:
		if (IsKeyPressed(KEY_ONE)) {
			audio->PlayFx(optSelectFx);
			LoadMap(Maps::MOSS_GROTTO_1);
		}
		if (IsKeyPressed(KEY_TWO)) {
			audio->PlayFx(optSelectFx);
			LoadMap(Maps::CRYSTAL_PEAK_1);
		}
		if (IsKeyPressed(KEY_THREE)) {
			audio->PlayFx(optSelectFx);
			LoadMap(Maps::MOSS_GROTTO_2);
		}
		if (IsKeyPressed(KEY_FOUR)) {
			audio->PlayFx(optSelectFx);
			LoadMap(Maps::CRYSTAL_PEAK_2);
		}
		if (IsKeyPressed(KEY_ENTER)) {
			audio->PlayFx(optSelectFx);
			switch (selected) {
			case 1:
				LoadMap(Maps::MOSS_GROTTO_1);
				map = Maps::MOSS_GROTTO_1;
				break;
			case 2:
				LoadMap(Maps::CRYSTAL_PEAK_1);
				map = Maps::CRYSTAL_PEAK_1;
				break;
			case 3:
				LoadMap(Maps::MOSS_GROTTO_2);
				map = Maps::MOSS_GROTTO_2;
				break;
			case 4:
				LoadMap(Maps::CRYSTAL_PEAK_2);
				map = Maps::CRYSTAL_PEAK_2;
				break;
			}
		}

		if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
			if (selected == 1) {
				selected = 4;
			}
			else {
				selected--;
			}
			audio->PlayFx(moveMenuFx);
		}
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
			if (selected == 4) {
				selected = 1;
			}
			else {
				selected++;
			}
			audio->PlayFx(moveMenuFx);
		}
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
			for (int i = 0; i < 2; i++) {
				if (selected - 1 < 1) {
					selected = 4;
				}
				else {
					selected--;
				}
			}
			audio->PlayFx(moveMenuFx);
		}
		else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
			for (int i = 0; i < 2; i++) {
				if (selected + 1 > 4) {
					selected = 1;
				}
				else {
					selected++;
				}
			}
			audio->PlayFx(moveMenuFx);
		}
		break;
	case Screens::GAME:
		if (playerLaps != player->myCar->laps) 
		{
			for (int i = 0; i < wormsPos.size(); i++) {
				worms.push_back(App->physics->CreateRectangle(wormsPos[i].x, wormsPos[i].y, 32, 32));
				worms[wormsPos.size() - 1]->ctype = ColliderType::WORM;
			}
			wormsPos.clear();
			playerLaps = player->myCar->laps;
		}

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
		playerTime = player->myCar->timer.ReadSec();
		DrawWorms();
		break;
	case Screens::END_RANK:
		SetCamera(1.0f, Vector2{0,0}, Vector2{ 0,0 });
		if (IsKeyPressed(KEY_ENTER)) {
			audio->PlayFx(optSelectFx);
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

	for (PhysBody* body : toDelete)
	{
		auto it = std::find(worms.begin(), worms.end(), body);
		if (it != worms.end())
			worms.erase(it);

		App->physics->DeleteBody(body);
	}

	toDelete.clear();


	collidingEntities.clear();
	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	Car* car = (Car*)bodyA->entity;
	int x, y;
	switch (bodyB->ctype)
	{
	case ColliderType::CHECKPOINT:
		CalculatePositions();
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
	case ColliderType::WORM:
		car->maxVelocity = 7.2f;
		car->turboTime.Start();
		bodyB->GetPhysicPosition(x, y);
		wormsPos.push_back(Vector2{ (float)x,(float)y });
		toDelete.push_back(bodyB);
		App->audio->PlayFx(turboFx);
		break;
	default:
		break;
	}
}

void ModuleGame::LoadMap(Maps _map) {
	wormsPos.clear();
	carsRanking.clear();
	switch (_map) {
	case Maps::MOSS_GROTTO_1:
		currentScreen = Screens::GAME;
		App->renderer->backgroundTexture = LoadTexture("Assets/Maps/MossGrotto.png");
		EXTERIOR = App->physics->CreateChain(0, 0, MossGrottoEXT, 196);
		EXTERIOR->body->SetType(b2BodyType::b2_staticBody);
		INTERIOR = App->physics->CreateChain(0, 0, MossGrottoINT, 156);
		INTERIOR->body->SetType(b2BodyType::b2_staticBody);
		CheckPoint1 = App->physics->CreateRectangleSensor(155, 620, 210, 30);
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
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(400, 800, 150, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(375, 600, 100, 100));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(525, 550, 100, 100));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(525, 150, 200, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(100, 250, 200, 400));


			//Auxiliary
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(850, 920, 50, 50));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(850, 1120, 200, 350));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(500, 1105, 400, 250));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(470, 640, 25, 25));

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
			//Auxiliary
			enemies[i]->turnLeft[7]->identifier = 2;
			enemies[i]->turnLeft[8]->identifier = 1;
			enemies[i]->turnLeft[9]->identifier = 1;
			enemies[i]->turnRight[3]->identifier = 0;
		}

		worms.push_back(App->physics->CreateRectangle(500, 1100, 32, 32));
		worms.push_back(App->physics->CreateRectangle(680, 302, 32, 32));
		worms.push_back(App->physics->CreateRectangle(470, 394, 32, 32));
		App->audio->PlayMusic("Assets/Audio/Music/GrassMap.mp3");
		break;

	case Maps::MOSS_GROTTO_2:
		currentScreen = Screens::GAME;
		App->renderer->backgroundTexture = LoadTexture("Assets/Maps/MossGrotto_2.png");
		EXTERIOR = App->physics->CreateChain(0, 0, MossGrotto_2EXT, 134);
		EXTERIOR->body->SetType(b2BodyType::b2_staticBody);
		INTERIOR = App->physics->CreateChain(0, 0, MossGrotto_2INT, 78);
		INTERIOR->body->SetType(b2BodyType::b2_staticBody);
		EXTRAS = App->physics->CreateChain(0, 0, MossGrotto_2EXTRAS, 38);
		EXTRAS->body->SetType(b2BodyType::b2_staticBody);
		CheckPoint1 = App->physics->CreateRectangleSensor(125, 783, 230, 30);
		CheckPoint1->identifier = 1;
		CheckPoint2 = App->physics->CreateRectangleSensor(1380, 1092, 300, 30);
		CheckPoint3 = App->physics->CreateRectangleSensor(981, 700, 30, 240);
		CheckPoint4 = App->physics->CreateRectangleSensor(254, 135, 30, 240);
		CheckPoint1->ctype = ColliderType::CHECKPOINT;
		CheckPoint2->ctype = ColliderType::CHECKPOINT;
		CheckPoint3->ctype = ColliderType::CHECKPOINT;
		CheckPoint4->ctype = ColliderType::CHECKPOINT;


		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(135, 1230, 290, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(635, 1220, 340, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1350, 1200, 220, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1350, 970, 220, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(900, 965, 320, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(450, 1000, 220, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(450, 700, 220, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1360, 720, 260, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1330, 490, 260, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(680, 470, 260, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(400, 480, 170, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(385, 200, 220, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(125, 250, 220, 200));

			enemies[i]->turnLeft[0]->identifier = 1; 
			enemies[i]->turnLeft[1]->identifier = 1; 
			enemies[i]->turnLeft[2]->identifier = 0; 
			enemies[i]->turnLeft[3]->identifier = 3;
			enemies[i]->turnLeft[4]->identifier = 3;

			enemies[i]->turnRight[0]->identifier = 0; 
			enemies[i]->turnRight[1]->identifier = 1;

			enemies[i]->turnLeft[5]->identifier = 0;
			enemies[i]->turnLeft[6]->identifier = 3;
			enemies[i]->turnLeft[7]->identifier = 3;

			enemies[i]->turnRight[2]->identifier = 0;
			enemies[i]->turnLeft[8]->identifier = 3;
			enemies[i]->turnLeft[9]->identifier = 2;
		}
		worms.push_back(App->physics->CreateRectangle(280, 1236, 32, 32));
		worms.push_back(App->physics->CreateRectangle(420, 822, 32, 32));
		worms.push_back(App->physics->CreateRectangle(970, 478, 32, 32));
		worms.push_back(App->physics->CreateRectangle(742, 114, 32, 32));
		App->audio->PlayMusic("Assets/Audio/Music/bellhart.mp3");
		break;


	case Maps::CRYSTAL_PEAK_1:
		currentScreen = Screens::GAME;
		App->renderer->backgroundTexture = LoadTexture("Assets/Maps/CrystalPeak.png");
		EXTERIOR = App->physics->CreateChain(0, 0, CrystalPeakEXT, 174);
		EXTERIOR->body->SetType(b2BodyType::b2_staticBody);
		INTERIOR = App->physics->CreateChain(0, 0, CrystalPeakINT, 168);
		INTERIOR->body->SetType(b2BodyType::b2_staticBody);
		CheckPoint1 = App->physics->CreateRectangleSensor(100, 720, 210, 30);
		CheckPoint1->identifier = 1;
		CheckPoint2 = App->physics->CreateRectangleSensor(425, 500, 210, 30);
		CheckPoint3 = App->physics->CreateRectangleSensor(500, 75, 30, 150);
		CheckPoint4 = App->physics->CreateRectangleSensor(1175, 700, 250, 30);
		CheckPoint1->ctype = ColliderType::CHECKPOINT;
		CheckPoint2->ctype = ColliderType::CHECKPOINT;
		CheckPoint3->ctype = ColliderType::CHECKPOINT;
		CheckPoint4->ctype = ColliderType::CHECKPOINT;

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(115, 1180, 290, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(475, 1180, 250, 400));

			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(455, 305, 250, 220));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(955, 300, 125, 300));

			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(935, 600, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(635, 600, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(615, 820, 210, 130));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(885, 800, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(755, 1000, 250, 200));

			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1230, 1000, 250, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1230, 90, 250, 200));

			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(200, 60, 250, 200));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(220, 240, 200, 130));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(30, 260, 150, 200));

			enemies[i]->turnLeft[0]->identifier = 1; 
			enemies[i]->turnLeft[1]->identifier = 0; 
			enemies[i]->turnLeft[2]->identifier = 1; 
			enemies[i]->turnRight[0]->identifier = 1; 
			enemies[i]->turnRight[1]->identifier = 2; 
			enemies[i]->turnRight[2]->identifier = 3; 
			enemies[i]->turnLeft[3]->identifier = 2;
			enemies[i]->turnLeft[4]->identifier = 1;
			enemies[i]->turnRight[3]->identifier = 0;

			enemies[i]->turnLeft[5]->identifier = 0;
			enemies[i]->turnLeft[6]->identifier =3;

			enemies[i]->turnLeft[7]->identifier = 2;
			enemies[i]->turnRight[4]->identifier = 3;
			enemies[i]->turnLeft[8]->identifier = 2;
		}
		worms.push_back(App->physics->CreateRectangle(360, 1014, 32, 32));
		worms.push_back(App->physics->CreateRectangle(1194, 1126, 32, 32));
		worms.push_back(App->physics->CreateRectangle(714, 52, 32, 32));
		App->audio->PlayMusic("Assets/Audio/Music/crystalPeak.mp3");
		break;
	case Maps::CRYSTAL_PEAK_2:
		currentScreen = Screens::GAME;
		App->renderer->backgroundTexture = LoadTexture("Assets/Maps/CrystalPeak_2.png");
		EXTERIOR = App->physics->CreateChain(0, 0, CrystalPeak_2EXT, 158);
		EXTERIOR->body->SetType(b2BodyType::b2_staticBody);
		INTERIOR = App->physics->CreateChain(0, 0, CrystalPeak_2INT, 134);
		INTERIOR->body->SetType(b2BodyType::b2_staticBody);
		CheckPoint1 = App->physics->CreateRectangleSensor(124, 845, 225, 30);
		CheckPoint1->identifier = 1;
		CheckPoint2 = App->physics->CreateRectangleSensor(650, 910, 210, 30);
		CheckPoint3 = App->physics->CreateRectangleSensor(1150, 640, 230, 30);
		CheckPoint4 = App->physics->CreateRectangleSensor(306, 105, 30, 195);
		CheckPoint1->ctype = ColliderType::CHECKPOINT;
		CheckPoint2->ctype = ColliderType::CHECKPOINT;
		CheckPoint3->ctype = ColliderType::CHECKPOINT;
		CheckPoint4->ctype = ColliderType::CHECKPOINT;

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(115, 1180, 290, 200));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(465, 1180, 200, 400));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(380, 740, 220, 180));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(650, 740, 200, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(660, 1180, 200, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(900, 1180, 200, 180));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(900, 760, 200, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1150, 760, 200, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(1120, 470, 200, 180));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(850, 420, 240, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(810, 100, 300, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(170, 110, 230, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(170, 300, 230, 180));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(575, 300, 200, 180));
			enemies[i]->turnRight.push_back(App->physics->CreateRectangleSensor(575, 500, 200, 180));
			enemies[i]->turnLeft.push_back(App->physics->CreateRectangleSensor(175, 505, 200, 180));

			enemies[i]->turnLeft[0]->identifier = 1;
			enemies[i]->turnLeft[1]->identifier = 0;

			enemies[i]->turnRight[0]->identifier = 1;
			enemies[i]->turnRight[1]->identifier = 2;

			enemies[i]->turnLeft[2]->identifier = 1;
			enemies[i]->turnLeft[3]->identifier = 0;

			enemies[i]->turnRight[2]->identifier = 1;

			enemies[i]->turnLeft[4]->identifier = 0;
			enemies[i]->turnLeft[5]->identifier = 3;

			enemies[i]->turnRight[3]->identifier = 0;

			enemies[i]->turnLeft[6]->identifier = 3;
			enemies[i]->turnLeft[7]->identifier = 2;
			enemies[i]->turnLeft[8]->identifier = 1;

			enemies[i]->turnRight[4]->identifier = 2;
			enemies[i]->turnRight[5]->identifier = 3;

			enemies[i]->turnLeft[9]->identifier = 2;
		}

		worms.push_back(App->physics->CreateRectangle(292, 958, 32, 32));
		worms.push_back(App->physics->CreateRectangle(958, 888, 32, 32));
		worms.push_back(App->physics->CreateRectangle(142, 206, 32, 32));
		App->audio->PlayMusic("Assets/Audio/Music/bellhart.mp3");
		break;

	case Maps::BELLHART_1:
		currentScreen = Screens::GAME;
		break;
	case Maps::BELLHART_2:
		currentScreen = Screens::GAME;
		break;
	}
	SetUpCars();
	for (int i = 0; i < worms.size(); i++) {
		worms[i]->ctype = ColliderType::WORM;
	}
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
		selected = 1;
		App->audio->PlayMusic("Assets/Audio/Music/cancoInicial.mp3");
		App->renderer->backgroundTexture = LoadTexture("Assets/UI/Main_Menu.png");
		break;
	case Screens::CHAR_SELECT:
		selected = 1;
		App->audio->PlayMusic("Assets/Audio/Music/SelectScreen.mp3");
		App->renderer->backgroundTexture = LoadTexture("Assets/UI/Select_Racer.png");
		App->renderer->Animate(App->renderer->selKnight, Vector2{ 106,22 }, 6, 4);
		//edit
		break;
	case Screens::MAP_SELECT:
		selected = 1;
		App->renderer->backgroundTexture = LoadTexture("Assets/UI/Map_Select.png");
		break;
	case Screens::GAME:
		LoadMap(map); 
		break;
	case Screens::END_RANK:
		App->audio->PlayMusic("Assets/Audio/Music/SelectScreen.mp3");
		App->renderer->backgroundTexture = LoadTexture("Assets/UI/Ranking_black.png");
		break;
	}
}

void ModuleGame::UnloadGame() {
	if (map == Maps::MOSS_GROTTO_1) {
		bestTime = &bestTimeM1;
	}
	else if (map == Maps::MOSS_GROTTO_2) {
		bestTime = &bestTimeM2;
	}
	else if (map == Maps::CRYSTAL_PEAK_1) {
		bestTime = &bestTimeC1;
	}
	else {
		bestTime = &bestTimeC2;
	}
	if (playerTime < *bestTime || *bestTime == 0) *bestTime = playerTime;
	if (cars[0] == player->myCar) {
		audio->PlayFx(winFX);
	}
	else {
		audio->PlayFx(looseFX);
	}
	App->physics->DeleteBody(EXTERIOR);
	App->physics->DeleteBody(INTERIOR);
	if (EXTRAS != nullptr) { App->physics->DeleteBody(EXTRAS); }
	App->physics->DeleteBody(CheckPoint1);
	App->physics->DeleteBody(CheckPoint2);
	App->physics->DeleteBody(CheckPoint3);
	App->physics->DeleteBody(CheckPoint4);
	for (Car* car : cars) {
		carsRanking.push_back(*car); // còpia independent
	}
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
	for (int i = 0; i < worms.size(); i++) {
		toDelete.push_back(worms[i]);
	}

	cars.clear();

}

void ModuleGame::SetCamera(float zoom, Vector2 offset, Vector2 target) {
	App->renderer->camera.zoom = zoom;
	App->renderer->camera.offset = offset;
	App->renderer->camera.target = target;
}
//printf("\n%d, ", (int)cars[i]->character);
//printf("%d\n ", (int)carsRanking[0].character);
void ModuleGame::SetUpCars() {
	cars.clear();
	player->myCar = new Car(App->physics, 100, 600, App->scene_intro, player->carText);
	carSetup(player->myCar, &player->character);
	cars.push_back(player->myCar);
	enemy1->character = Characters::KNIGHT;
	enemy2->character = Characters::ZOTE;
	enemy3->character = Characters::SHERMA;
	enemy4->character = Characters::P_KING;
	enemy5->character = Characters::PABLO;
	enemy1->myCar = new Car(App->physics, 140, 575, App->scene_intro, enemy1->carText);
	enemy2->myCar = new Car(App->physics, 180, 550, App->scene_intro, enemy2->carText);
	enemy3->myCar = new Car(App->physics, 100, 525, App->scene_intro, enemy3->carText);
	enemy4->myCar = new Car(App->physics, 140, 500, App->scene_intro, enemy4->carText);
	enemy5->myCar = new Car(App->physics, 180, 475, App->scene_intro, enemy5->carText);
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->turnTargetRotation = 180;
		if (enemies[i]->character == player->character) {
			enemies[i]->character = Characters::HORNET;
		}
		carSetup(enemies[i]->myCar, &enemies[i]->character);
		cars.push_back(enemies[i]->myCar);
	}
}

void ModuleGame::CalculatePositions() {
	int n = cars.size();

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (cars[j]->laps < cars[j + 1]->laps) {
				std::swap(cars[j], cars[j + 1]);
			}
			else if (cars[j]->laps == cars[j + 1]->laps &&
				cars[j]->checkPoints.size() < cars[j + 1]->checkPoints.size()) {
				std::swap(cars[j], cars[j + 1]);
			}
		}
	}
}

void ModuleGame::DrawWorms() {
	for (int i = 0; i < worms.size(); i++) {
		int x, y;
		worms[i]->GetPhysicPosition(x, y);
		DrawTextureEx(wormTEXT, { (float)x, (float)y }, 0, 1, WHITE);
	}
}