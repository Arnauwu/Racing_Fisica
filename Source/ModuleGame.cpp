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
	player = new ModulePlayer(App, true, 100, 400);
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.zoom = 1.0f;
	App->renderer->camera.target = { 0.0f, 0.0f };
	App->renderer->camera.offset = { 0.0f, 0.0f };
	App->renderer->camera.rotation = 0.0f;

	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	App->renderer->backgroundTexture = LoadTexture("Assets/Maps/MossGrotto.png");
	App->physics->CreateChain(0, 0, MossGrottoEXT, 196);
	App->physics->CreateChain(0, 0, MossGrottoINT, 156);
	CheckPoint1 = App->physics->CreateRectangleSensor(155, 600, 210, 30);
	CheckPoint2 = App->physics->CreateRectangleSensor(1140, 850, 300, 30);
	CheckPoint3 = App->physics->CreateRectangleSensor(725, 360, 150, 30);
	CheckPoint4 = App->physics->CreateRectangleSensor(525, 350, 210, 30);
	CheckPoint1->ctype = ColliderType::CHECKPOINT;
	CheckPoint2->ctype = ColliderType::CHECKPOINT;
	CheckPoint3->ctype = ColliderType::CHECKPOINT;
	CheckPoint4->ctype = ColliderType::CHECKPOINT;

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
	int playerX, playerY;
	player->myCar->body->GetPhysicPosition(playerX, playerY);

	float currentZoom = App->renderer->camera.zoom = 1.5f;

	float halfScreenWidth = SCREEN_WIDTH / 2.0f;
	float halfScreenHeight = SCREEN_HEIGHT / 2.0f;

	float visibleHalfWidth = halfScreenWidth / currentZoom;
	float visibleHalfHeight = halfScreenHeight / currentZoom;

	App->renderer->camera.offset = Vector2{ halfScreenWidth, halfScreenHeight };

	float targetX = (float)playerX;
	float targetY = (float)playerY;


	App->renderer->camera.target = Vector2{ targetX, targetY };

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
		if (car->checkPoints.size() < 4) {
			bool isThere = false;
			for (int i = 0; i < car->checkPoints.size(); i++) {
				if (car->checkPoints.at(i) == bodyB) isThere = true;
			}
			if (!isThere) car->checkPoints.push_back(bodyB);
		}
		else {
			car->checkPoints.clear();
			car->checkPoints.push_back(bodyB);
			car->laps++;
		}
		printf("%d", car->checkPoints.size());
		break;
	case ColliderType::TURBO:

		break;
	default:
		break;
	}
}