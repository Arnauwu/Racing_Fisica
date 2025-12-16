#include "Car.h"

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
		Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
}

int Car::RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
{
	return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
}

void Car::Move() {
	int x, y;
	body->GetPhysicPosition(x, y);
	if (body->body->GetLinearVelocity().y > -1.0f) {
		body->body->ApplyLinearImpulse(limitV, { float(x), float(y)}, true);
	}
}