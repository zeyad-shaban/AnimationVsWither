// added comment here
#include "raylib.h"

#ifndef ANIM_IMG_H
#define ANIM_IMG_H

class AnimImg {
   public:
    AnimImg(char* path, float _maxCols, float _scale, float _maxRows = 1.f) {
        maxCols = _maxCols;
        maxRows = _maxRows;
        scale = _scale;
        texture = LoadTexture(path);
        width = texture.width / maxCols;
        height = texture.height / maxRows;

        src = {0, 0, width, height};
        dest = {0, 0, width * scale, height * scale};
    }

    void render(float delta, Vector2 spawn) {
        runningTime += delta;
        if (runningTime >= updateTime) {
            runningTime = 0;
            frame.x++;
            if (frame.x >= maxCols) {
                frame.x = 0;
                frame.y++;
                if (frame.y >= maxRows) frame.y = 0;
            }
        }

        src.x = frame.x * width;
        src.y = frame.y * height;
        dest.x = spawn.x;
        dest.y = spawn.y;
        DrawTexturePro(texture, src, dest, Vector2{}, 0.f, WHITE);
    }

    float getWidth() { return width * scale; }
    float getHeight() { return height * scale; }

   private:
    Texture2D texture;
    Rectangle src;
    Rectangle dest;
    float maxRows;
    float maxCols = 8.f;
    float scale = 0.1f;
    Vector2 frame{};
    float runningTime = 0;
    float updateTime = 1 / 12.f;
    float width;
    float height;
    // sword end
};

#endif