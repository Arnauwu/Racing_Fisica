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

	if (carRotation >= 360) 
	{ 
		carRotation = 0; 
		impulse.y = -1.8f;
		impulse.x = 0;
	}
	if (carRotation < 0) 
	{ 
		carRotation = 359; 
		impulse.y = -1.7f;
		impulse.x = -0.1f;
	}
	if (carRotation == 270)
	{
		impulse.y = 0;
		impulse.x = -1.8f;
	}
	else if (carRotation == 90){
		impulse.y = 0;
		impulse.x = 1.8f;
	}

	body->body->SetLinearVelocity(impulse);
}