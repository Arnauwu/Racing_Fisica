#include "Map.h"

enum TileType
{
    TILE_EMPTY = 0,
    TILE_WALL = 1
};

void Map::LoadFromPNG(const std::string& path)
{
    Image image = LoadImage(path.c_str());
    if (image.data == nullptr)
    {
        TraceLog(LOG_ERROR, "Failed to load map PNG: %s", path.c_str());
        return;
    }

    width = image.width;
    height = image.height;

    tiles.clear();
    tiles.resize(width * height);

    Color* pixels = LoadImageColors(image);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const Color c = pixels[y * width + x];

            // Black = wall, anything else = empty
            tiles[y * width + x] =
                (c.r == 0 && c.g == 0 && c.b == 0)
                ? TILE_WALL
                : TILE_EMPTY;
        }
    }

    UnloadImageColors(pixels);
    UnloadImage(image);
}

void Map::Draw() const
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (tiles[y * width + x] == TILE_WALL)
            {
                DrawRectangle(
                    x * tileSize,
                    y * tileSize,
                    tileSize,
                    tileSize,
                    DARKGRAY
                );
            }
        }
    }
}

void Map::Unload()
{
    tiles.clear();
    width = height = 0;
}