#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

public:
	PhysBody* body;
	Module* listener;
};

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = LoadTexture("Assets/wheel.png");
	box = LoadTexture("Assets/crate.png");
	rick = LoadTexture("Assets/rick_head.png");

	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

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
	if (IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if (IsKeyPressed(KEY_ONE))
	{
		

	}

	if (IsKeyPressed(KEY_TWO))
	{
		
	}

	if (IsKeyPressed(KEY_THREE))
	{
		
	}

	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	for (PhysicEntity* entity : entities)
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
		for (PhysicEntity* entity : collision)
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
	if (!bodyA || !bodyB)
	{
		return;
	}

	bool isCircle1 = false;
	bool isCircle2 = false;


	const b2Fixture* fixture = bodyA->body->GetFixtureList();
	while (fixture != NULL)
	{
		if (const b2CircleShape* circle = dynamic_cast<const b2CircleShape*>(fixture->GetShape()))
		{
			//sumradius += circle->m_radius;
			isCircle1 = true;
			break;
		}
		fixture = fixture->GetNext();
	}

	const b2Fixture* fixture2 = bodyB->body->GetFixtureList();
	while (fixture2 != NULL)
	{
		if (const b2CircleShape* circle = dynamic_cast<const b2CircleShape*>(fixture2->GetShape()))
		{
			//sumradius += circle->m_radius;
			isCircle2 = true;
			break;
		}
		fixture2 = fixture2->GetNext();
	}

	if (isCircle1 && isCircle2)
	{
		std::set<PhysicEntity*> colliding;
		//Delete both bodies
		for (PhysicEntity* entity : entities)
		{
			if (entity->body == bodyA || entity->body == bodyB)
			{
				colliding.emplace(entity);
			}
		}
		collidingEntities.emplace(colliding);
	}
}