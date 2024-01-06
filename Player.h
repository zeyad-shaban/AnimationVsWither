#include "AnimImg.h"
#include "Stage.h"
#include "raylib.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
   public:
    Player(char *path, float spawnX, float spawnY);
    void render(float delta, Rectangle *platforms, int platformsSize, Sound jumpSound);
    Vector2 getWorldPos() { return worldPos; }
    bool checkLava(Rectangle *lavas, int lavasSize);
    int updateHealth(int amount);
    void restartPosition();
    void displayHearts(float delta);
    void setStage(Stage _stage);
    Rectangle getHitbox();
    bool getCanAttack() { return canAttack; }
    float getWidth() { return width; }

   private:
    void animate(float maxFrames, float row);
    bool canAttack = false;

    Texture2D texture;
    float width;
    float height;
    float maxRows = 3;
    float maxCols = 12;

    float scale = 2.f;
    float baseSpeed = 350;
    float speed;
    float baseJumpPower = -900;
    float gravity = 1500;
    float velocityY = 0;
    float jumpPower;
    bool isInAir = false;
    float maxFallVel = 1000.f;

    float paddingX = 50;
    float paddingY = 15;

    float spawnX;
    float spawnY;

    // animating vars
    float updateTime = 1.f / 12.f;
    float runningTime = 0;
    float frame = 0;

    float idleRow = 0, idleFrames = 7;
    float jumpRow = 1, jumpFrames = 12;
    float runRow = 2, runFrames = 5;
    float attackRow = 1, attackFrames = 7;
    // end animating vars

    AnimImg heartAnim = {"assets/images/heart.png", 6, 0.3f};
    int hearts = 2;
    float spawnProtectionMax = 2.f;
    float spawnProtection = spawnProtectionMax;
    Sound screamSound = LoadSound("./assets/sounds/scream.mp3");

    Rectangle src;
    Rectangle dest;
    Vector2 worldPos{};
};

#endif