#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>
#include <limits.h>
#include "Timer.h"

class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void ClearUI();

    void SetBackgroundColor(Color color);
	bool Animate(Texture2D texture, Vector2 pos, int totalFrames, int frameSpeed);
	bool Draw(Texture2D texture, int x, int y, const Rectangle* section = NULL, double* angle = 0, float scale = 0.0, int pivot_x = 0, int pivot_y = 0) const;
	bool DrawText(const char* text, int x, int y, Font font, int spacing, Color tint) const;

	UIElement* CreateButton(int id, Rectangle bounds, const char* text, Module* observer);
	std::vector<UIElement*> uiElements;

public:

	Color background;
	Texture2D backgroundTexture;
	Texture2D icons;
	Texture2D menuButton;
	Texture2D charButton;
	Texture2D mapButton;
    Camera2D camera;

	Texture2D selKnight;
	Texture2D selHornet;
	Texture2D selZote;
	Texture2D selSherma;
	Texture2D selPaleKing;
	Texture2D selPablo;

	Rectangle frameRec;

	Timer animTimer;
};