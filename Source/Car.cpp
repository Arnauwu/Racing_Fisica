#include "Car.h"
#include "math.h"
#include "Application.h"
#include "ModuleRender.h"
void Car::Update()
{
	CheckFinishLane();
	if (textTiles == 0) {
		textTiles = texture.width / 32;
	}
	int x, y;
	body->GetPhysicPosition(x, y);
	if (frameCount > 2) {
		frameCount = 0;
		drawFrame += 32;
	}
	if (drawFrame / 32 > textTiles || body->body->GetLinearVelocity() == b2Vec2_zero) {
		drawFrame = 0;
	}
	if (turboTime.ReadSec() > 1 && maxVelocity == 7.2f) {
		maxVelocity = 3.6f;
	}
	int charFrame = (int)*character;
	const Rectangle section = { drawFrame, charFrame * 32, 32, 32 };
	App->renderer->Draw(texture, x, y, &section, &carRotation, scale);
	frameCount++;
	Move();
}

int Car::RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
{
	return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
}

void Car::Move() {
	int x, y;
	body->GetPhysicPosition(x, y);

	body->body->SetLinearVelocity(impulse);
}

void Car::CheckFinishLane() {
	if (laps == 3) 
	{
		printf("META META META META META\n");
	}
}