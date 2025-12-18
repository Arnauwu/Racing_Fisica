#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class Map
{
public:
    void LoadFromPNG(const std::string& path);
    void Draw() const;
    void Unload();

private:
    int width = 0;
    int height = 0;
    int tileSize = 32;

    std::vector<int> tiles;
};