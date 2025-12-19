#include "Globals.h"
#include "Application.h"
#include "ModuleGame.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "UIButton.h"
#include "Map.h"
#include <math.h>

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	bool ret = true;
    background = GRAY;
    backgroundTexture = LoadTexture("Assets/Maps/MossGrotto.png");
    icons = LoadTexture("Assets/Characters/icons_vertical.png");

	return ret;
}


// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
    Rectangle rectangleBg = { 0,0,1280,1280 };

    return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
{
    ClearBackground(background);

    // NOTE: This function setups render batching system for
    // maximum performance, all consecutive Draw() calls are
    // not processed until EndDrawing() is called
    BeginDrawing();

    //NOT CAMERA, DOWN
    BeginMode2D(camera);
    DrawTexture(backgroundTexture, 0, 0, WHITE);
    EndMode2D();

    //NOT CAMERA, UP
    DrawTextureEx(icons, {1230, 0},0,3, WHITE);
    DrawFPS(10, 10);

    //CAMERA
    BeginMode2D(camera);

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
    // Draw everything in our batch!
    EndDrawing();

	return UPDATE_CONTINUE;
}

// Called before quitting
void ModuleRender::ClearUI()
{
    for (UIElement* element : uiElements)
    {
        delete element;
    }
    uiElements.clear();
}

bool ModuleRender::CleanUp()
{
	return true;
}

void ModuleRender::SetBackgroundColor(Color color)
{
	//background = color;
}

UIElement* ModuleRender::CreateButton(int id, Rectangle bounds, const char* text, Module* observer)
{
    UIButton* newButton = new UIButton(id, bounds, text);

    newButton->SetObserver(observer);

    uiElements.push_back(newButton);

    return newButton;
}

// Draw to screen
bool ModuleRender::Draw(Texture2D texture, int x, int y, const Rectangle* section, double* angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	float scale = 1.0f;
    Vector2 position = { (float)x, (float)y };
    Rectangle rect = { 0.f, 0.f, (float)texture.width, (float)texture.height };

    if (section != NULL) rect = *section;

    position.x = (float)(x-pivot_x) * scale;
    position.y = (float)(y-pivot_y) * scale;

	rect.width *= scale;
	rect.height *= scale;

    if (angle == nullptr) {
        DrawTextureRec(texture, rect, position, WHITE);
    }
    else {
        int textTiles = texture.width / 32;
        DrawTexturePro(texture, *section,
            Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
            Vector2{ (float)texture.width / textTiles, (float)texture.height / textTiles }, (float)*angle, WHITE);
    }

	return ret;
}

bool ModuleRender::DrawText(const char * text, int x, int y, Font font, int spacing, Color tint) const
{
    bool ret = true;

    Vector2 position = { (float)x, (float)y };

    DrawTextEx(font, text, position, (float)font.baseSize, (float)spacing, tint);

    return ret;
}

