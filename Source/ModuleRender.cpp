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
    icons = LoadTexture("Assets/Characters/icons_vertical.png");
    menuButton = LoadTexture("Assets/UI/MenuButton.png");
    charButton = LoadTexture("Assets/UI/CharButton.png");
    mapButton = LoadTexture("Assets/UI/MapButton.png");

    selKnight = LoadTexture("Assets/Characters/selKnight_spritesheet.png");
    selHornet = LoadTexture("Assets/Characters/selHornet_spritesheet.png");
    selZote = LoadTexture("Assets/Characters/selZote_spritesheet.png");
    selSherma = LoadTexture("Assets/Characters/selSherma_spritesheet.png");
    selPaleKing = LoadTexture("Assets/Characters/selPaleKing_spritesheet.png");
    selPablo = LoadTexture("Assets/Characters/selPablo_spritesheet.png");

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
    if (App->scene_intro->currentScreen != Screens::GAME) {
        DrawTextureEx(backgroundTexture, Vector2{ 0,0 }, 0.0, 4.0, WHITE);
    }
    else {
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    }
    
    EndMode2D();

    //NOT CAMERA, UP
    if (App->scene_intro->currentScreen == Screens::GAME && App->scene_intro->cars.size() != 0) {
        /*DrawTextureEx(icons, { 1230, 0 }, 0, 3, WHITE);*/
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->cars[0]->character * 16, 16, 16 }, {1245, 100, 48, 48}, {(float)icons.width, (float)icons.height}, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->cars[1]->character * 16, 16, 16 }, { 1245, 148, 48, 48 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->cars[2]->character * 16, 16, 16 }, { 1245, 196, 48, 48 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->cars[3]->character * 16, 16, 16 }, { 1245, 244, 48, 48 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->cars[4]->character * 16, 16, 16 }, { 1245, 292, 48, 48 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->cars[5]->character * 16, 16, 16 }, { 1245, 340, 48, 48 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTextEx(GetFontDefault(), "1", { 1205,0 }, 45, 0, RAYWHITE);
        DrawTextEx(GetFontDefault(), "2", { 1200,50 }, 45, 0, RAYWHITE);
        DrawTextEx(GetFontDefault(), "3", { 1200,100 }, 45, 0, RAYWHITE);
        DrawTextEx(GetFontDefault(), "4", { 1200,150 }, 45, 0, RAYWHITE);
        DrawTextEx(GetFontDefault(), "5", { 1200,200 }, 45, 0, RAYWHITE);
        DrawTextEx(GetFontDefault(), "6", { 1200,250 }, 45, 0, RAYWHITE);
        DrawTextPro(GetFontDefault(), TextFormat("Time: %.3f", App->scene_intro->playerTime), { 1130, 300 }, { 0,0 }, 0, 30, 1, WHITE);
    }
    else if (App->scene_intro->currentScreen == Screens::MAIN_MENU) {
        int* option = &App->scene_intro->selected;
        Vector2 position = { 0, 0 };
        if (*option == 1) {
            position = { (SCREEN_WIDTH/2) - (float)(menuButton.width * 2), (SCREEN_HEIGHT / 2) + 48 };
        }
        else {
            position = { (SCREEN_WIDTH / 2) - (float)(menuButton.width * 2), (SCREEN_HEIGHT / 2) + 114};
        }
        DrawTextureEx(menuButton, position, 0, 4, WHITE);
    }
    else if (App->scene_intro->currentScreen == Screens::CHAR_SELECT) {
        
       
        
        int* option = &App->scene_intro->selected;
        Vector2 position = { 106, 22 * 4 };
        //7-9x 10y
        if (*option == 1 || *option == 4) {
            position.x = 106*4 - (7 * 2);
        } 
        else if(*option == 2 || *option == 5) {
            position.x = 106*4 + (7 * 2) + (64 * 4);
        }
        else if (*option == 3 || *option == 6) {
            position.x = 106 * 4 + (7 * 2) * 3 + (64 * 4) * 2;
        }
        if (*option > 3) {
            position.y = 22 * 4 - 10 + charButton.height * 4;
        }
        else {
            position.y = 22 * 4 - 10 * 2;
        }
        DrawTextureEx(charButton, position, 0, 4, WHITE);
      
        Vector2 anPos = { 106*4,21*4 };
        Texture2D selText;
        Rectangle* recPoint;

        DrawTexturePro(selKnight, frameRecK, Rectangle{ (float)anPos.x, (float)anPos.y, 4 * 64, 4 * 64 }, Vector2{ 0, 0 }, 0, WHITE);
        DrawTexturePro(selHornet, frameRecH, Rectangle{ (float)anPos.x + (71 * 4), (float)anPos.y, 4 * 64, 4 * 64 }, Vector2{ 0, 0 }, 0, WHITE);
        DrawTexturePro(selZote, frameRecZ, Rectangle{ (float)anPos.x + (71 * 4) * 2, (float)anPos.y, 4 * 64, 4 * 64 }, Vector2{ 0, 0 }, 0, WHITE);
        DrawTexturePro(selSherma, frameRecS, Rectangle{ (float)anPos.x, (float)anPos.y + (74 * 4), 4 * 64, 4 * 64 }, Vector2{ 0, 0 }, 0, WHITE);
        DrawTexturePro(selPaleKing, frameRecP, Rectangle{ (float)anPos.x + (71 * 4), (float)anPos.y + (74 * 4), 4 * 64, 4 * 64 }, Vector2{ 0, 0 }, 0, WHITE);
        DrawTexturePro(selPablo, frameRecP2, Rectangle{ (float)anPos.x + (71 * 4) * 2, (float)anPos.y + (74 * 4), 4 * 64, 4 * 64 }, Vector2{ 0, 0 }, 0, WHITE);

        if (*option == 1) {
            frameRecH = { 0, 0, 64, 64 };
            frameRecZ = { 0, 0, 64, 64 };
            frameRecS = { 0, 0, 64, 64 };
            frameRecP = { 0, 0, 64, 64 };
            frameRecP2 = { 0, 0, 64, 64 };
            recPoint = &frameRecK;
        }
        else if (*option == 2) {
            frameRecK = { 0, 0, 64, 64 };
            frameRecZ = { 0, 0, 64, 64 };
            frameRecS = { 0, 0, 64, 64 };
            frameRecP = { 0, 0, 64, 64 };
            frameRecP2 = { 0, 0, 64, 64 };
            recPoint = &frameRecH;
        }
        else if (*option == 3) {
            frameRecH = { 0, 0, 64, 64 };
            frameRecK = { 0, 0, 64, 64 };
            frameRecS = { 0, 0, 64, 64 };
            frameRecP = { 0, 0, 64, 64 };
            frameRecP2 = { 0, 0, 64, 64 };
            recPoint = &frameRecZ;
        }
        else if (*option == 4) {
            frameRecH = { 0, 0, 64, 64 };
            frameRecZ = { 0, 0, 64, 64 };
            frameRecK = { 0, 0, 64, 64 };
            frameRecP = { 0, 0, 64, 64 };
            frameRecP2 = { 0, 0, 64, 64 };
            recPoint = &frameRecS;
        }
        else if (*option == 5) {
            frameRecH = { 0, 0, 64, 64 };
            frameRecZ = { 0, 0, 64, 64 };
            frameRecS = { 0, 0, 64, 64 };
            frameRecK = { 0, 0, 64, 64 };
            frameRecP2 = { 0, 0, 64, 64 };
            recPoint = &frameRecP;
        }
        else {
            frameRecH = { 0, 0, 64, 64 };
            frameRecZ = { 0, 0, 64, 64 };
            frameRecS = { 0, 0, 64, 64 };
            frameRecP = { 0, 0, 64, 64 };
            frameRecK = { 0, 0, 64, 64 };
            recPoint = &frameRecP2;
        }

        if (recPoint->x > 320) {//320 és el nombre de frames. L'he comptat jo de la spritesheeet, però es podria fer com ho vaig fer al Car, que va automàtic.
            recPoint->x = 0;//Si el frame es passa del maxim de frames torna al 1r frame
        }
        if (animTimer.ReadSec() > 0.1f) {//Actualitza el frame quan ha passat el temps (en segons)
            recPoint->x += 64;
            animTimer.Start();//Reinicia el timer
        }
    }
    else if (App->scene_intro->currentScreen == Screens::MAP_SELECT) {
        int* option = &App->scene_intro->selected;
        Vector2 position = { 280, 55 * 4 }; //x=82, y=52

        if (*option == 1 || *option == 3) {
            position.x = 280;
        }
        else if (*option == 2 || *option == 4) {
            position.x = 280 + (20 * 2) + (82 * 4);
        }
        if (*option > 2) {
            position.y = 55 * 4 + mapButton.height * 4 + 8;
        }
        else {
            position.y = 55 * 4;
        }
        DrawTextureEx(mapButton, position, 0, 4, WHITE);
        DrawTextPro(GetFontDefault(), TextFormat("Best time: %.3f", App->scene_intro->bestTimeM1), { 0, (float)(55 * 4) + mapButton.height * 2 }, { 0,0 }, 0, 30, 4, WHITE);
        DrawTextPro(GetFontDefault(), TextFormat("Best time: %.3f", App->scene_intro->bestTimeM2), {0, ((float)55 * 4 + mapButton.height * 4 + 8) + mapButton.height * 2 }, {0,0}, 0, 30, 4, WHITE);
        DrawTextPro(GetFontDefault(), TextFormat("Best time: %.3f", App->scene_intro->bestTimeC1), { 1010, (float)(55 * 4) + mapButton.height * 2 }, { 0,0 }, 0, 30, 4, WHITE);
        DrawTextPro(GetFontDefault(), TextFormat("Best time: %.3f", App->scene_intro->bestTimeC2), { 1010, ((float)55 * 4 + mapButton.height * 4 + 8) + mapButton.height * 2 }, { 0,0 }, 0, 30, 4, WHITE);
    }
    else if (App->scene_intro->currentScreen == Screens::END_RANK) {
        DrawTextPro(GetFontDefault(), TextFormat("%.3f", App->scene_intro->playerTime), { 200, 90 }, { 0,0 }, 0, 30, 4, WHITE);
        DrawTextPro(GetFontDefault(), TextFormat("%.3f", *App->scene_intro->bestTime), { 335, 137 }, { 0,0 }, 0, 30, 4, WHITE);
        for (int i = 0; i < App->scene_intro->carsRanking.size(); i++) {
           int character = (int)*App->scene_intro->carsRanking[i].character;
            switch (character) {
            case 0:
                DrawTextEx(GetFontDefault(), "KNIGHT", { 800,(125 + (float)(i * 90)) }, 45, 5, WHITE);
                break;
            case 1:
                DrawTextEx(GetFontDefault(), "HORNET", { 800,(125 + (float)(i * 90)) }, 45, 5, WHITE);
                break;
            case 2:
                DrawTextEx(GetFontDefault(), "ZOTE", { 800,(125 + (float)(i * 90)) }, 45, 5, WHITE);
                break;
            case 3:
                DrawTextEx(GetFontDefault(), "SHERMA", { 800,(125 + (float)(i * 90)) }, 45, 5, WHITE);
                break;
            case 4:
                DrawTextEx(GetFontDefault(), "PALE KING", { 800,(125 + (float)(i * 90)) }, 45, 5, WHITE);
                break;
            case 5:
                DrawTextEx(GetFontDefault(), "PABLO", { 800,(125 + (float)(i * 90)) }, 45, 5, WHITE);
                break;
            default:
                break;
            }
        }
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->carsRanking[0].character * 16, 16, 16 }, { 700, 210, 64, 64 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->carsRanking[1].character * 16, 16, 16 }, { 700, 210 + 90, 64, 64 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->carsRanking[2].character * 16, 16, 16 }, { 700, 210 + 90 * 2, 64, 64 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->carsRanking[3].character * 16, 16, 16 }, { 700, 210 + 90 * 3, 64, 64 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->carsRanking[4].character * 16, 16, 16 }, { 700, 210 + 90 *4, 64, 64 }, { (float)icons.width, (float)icons.height }, 0, WHITE);
        DrawTexturePro(icons, { 0, 0 + (float)(int)*App->scene_intro->carsRanking[5].character * 16, 16, 16 }, { 700, 210 + 90 * 5, 64, 64 }, { (float)icons.width, (float)icons.height }, 0, WHITE);

        LOG("\nPERSONATGE PLAYER: %d\n",(int)App->scene_intro->player->character);

        int currentPlayer = (int)App->scene_intro->player->character;
        Vector2 rankPos = { 80,260 };
        Texture2D rankTexture;
        switch (currentPlayer) {
        case 0:
            rankTexture = selKnight;
            break;
        case 1:    
            rankTexture = selHornet;
            break;
        case 2:
            rankTexture = selZote;
            break;
        case 3:
            rankTexture = selSherma;
            break;
        case 4:
            rankTexture = selPaleKing;
            break;
        case 5:
            rankTexture = selPablo;
            break;
        default:
            break;
        }

        if (frameRec.x > 320) {//320 és el nombre de frames. L'he comptat jo de la spritesheeet, però es podria fer com ho vaig fer al Car, que va automàtic.
            frameRec.x = 0;//Si el frame es passa del maxim de frames torna al 1r frame
        }
        if (animTimer.ReadSec() > 0.1f) {//Actualitza el frame quan ha passat el temps (en segons)
            frameRec.x += 64;
            animTimer.Start();//Reinicia el timer
        }
        //4 ÉS LA SCALE:
        DrawTexturePro(rankTexture, frameRec, Rectangle{ (float)rankPos.x, (float)rankPos.y, 6.5 * 64, 6.5 * 64 }, Vector2{ 0, 0 }, 0, WHITE);

        DrawTextureEx(endTxt, { 60,190 }, 0.0, 4.0, WHITE);
    }

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
bool ModuleRender::Draw(Texture2D texture, int x, int y, const Rectangle* section, double* angle, float scale, int pivot_x, int pivot_y) const
{
	bool ret = true;

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
            Rectangle{ (float)x, (float)y, scale * 32, scale * 32 },
            Vector2{ scale * 16, scale * 16 }, (float)*angle, WHITE);
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

bool ModuleRender::Animate(Texture2D texture, Vector2 pos, int totalFrames, int frameSpeed) {
    bool ret = true;
    LOG("Entered Animate() successfuly");
    int* character = &App->scene_intro->selected;

    int currentFrame = 0;
    int frameContador = 0;
    frameRec = { 64,64 };

    //Animation loop
    //frameContador++;
    //if (frameContador >= (60 / frameSpeed)) {
    //    frameContador = 0;
    //    currentFrame++;
    //    
    //    if (currentFrame == totalFrames) {
    //        currentFrame = 0;
    //    }
    // frameRec.x = (float)currentFrame * 64;
    //}

    DrawTextureRec(texture, frameRec, pos, WHITE);
    LOG("Animated texture drawn");

    return ret;
}

