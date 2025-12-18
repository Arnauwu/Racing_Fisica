#include "Car.h"
#include "math.h"
#include "Application.h"
#include "ModuleRender.h"
void Car::Update()
{
	if (textTiles == 0) {
		textTiles = texture.width / 32;
	}
	int x, y;
	body->GetPhysicPosition(x, y);

	if (frameCount > 30) {
		frameCount = 0;
		drawFrame += 32;
	}
	if (drawFrame / 32 > textTiles || body->body->GetLinearVelocity() == b2Vec2_zero) {
		drawFrame = 0;
	}
	Move();
	const Rectangle section = { drawFrame, 0, (float)texture.width / textTiles, (float)texture.height };
	int playerX, playerY;
	App->renderer->Draw(texture, x, y, &section, &carRotation);
	frameCount++;
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
		impulse.y = -maxVelocity;
		impulse.x = 0;
	}
	if (carRotation < 0) 
	{ 
		carRotation = 359; 
	}
	if (carRotation == 270)
	{
		impulse.y = 0;
		impulse.x = -maxVelocity;
	}
	else if (carRotation == 90){
		impulse.y = 0;
		impulse.x = maxVelocity;
	}
	else if (carRotation == 180) {
		impulse.y = maxVelocity;
		impulse.x = 0;
	}

	body->body->SetLinearVelocity(impulse);


}