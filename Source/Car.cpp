#include "Car.h"
#include "math.h"
void Car::Update()
{
	if (textTiles == 0) {
		textTiles = texture.width / 32;
	}
	int x, y;
	body->GetPhysicPosition(x, y);

	Move();

	DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width/textTiles, (float)texture.height },
		Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
		Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, carRotation, WHITE);
}

int Car::RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
{
	return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
}

void Car::Move() {
	int x, y;
	body->GetPhysicPosition(x, y);
	body->body->SetAngularVelocity(5);
	if (body->body->GetLinearVelocity().y > -1.0f) {
		body->body->ApplyLinearImpulse({ car_x, car_y }, { float(x), float(y)}, true);
	}

	float radians = carRotation * DEG2RAD;
	float vx = cosf(radians);
	float vy = sinf(radians);

	car_x += vx * 0.1f;
	car_y -= vy * 0.1f;

	if (carRotation >= 360) { carRotation = 0; }

	if (IsKeyDown(KEY_A))
	{
		carRotation -= 5.0f;
	}
	else if (!IsKeyDown(KEY_D)) {
		carRotation += 5.0f;
	}
}