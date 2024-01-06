#include "Player.h"
#include "raylib.h"

#ifndef WITHER_SKELETON_H
#define WITHER_SKELETON_H

class WitherSkeleton {
   public:
    WitherSkeleton(char *path, float spawnX, float spawnY, int flip);
    void render(float delta, Player *target);
    Vector2 getWorldPos() { return worldPos; }
    bool throwSword(float delta, Player *target);
    void setStage(Stage stage);
    void hitSword(Vector2 p1, Vector2 p2);
    bool doLinesIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d);
    bool doesRectIntersectLine(Rectangle rect, Vector2 lineStart, Vector2 lineEnd);

   private:
    bool animate(float maxFrames, float row, bool loop);
    Rectangle getHitbox();
    Rectangle swordDest;

    Texture2D texture;
    float width;
    float height;
    float maxRows = 2;
    float maxCols = 52;

    float scale = 4.f;

    Vector2 padding{50, 15};
    Vector2 spawn{};

    // animating vars
    float updateTime = 1.f / 30.f;
    float runningTime = 0;
    float frame = 0;

    float attackRow = 0, attackFrames = 52;
    float idleRow = 1, idleFrames = 52;
    // end animating vars

    // sword begin
    Texture2D sword = LoadTexture("assets/images/wither-sword.png");
    Vector2 swordPos{};
    bool swordSpawned = false;
    float swordCols = 8.f;
    float swordScale = 0.1f;
    float swordSpeed = 2.f;
    float swordFrame = 0;
    float swordRunningTime;
    float swordUpdateTime = 1.f / 12.f;
    float swordMaxLifetime = 3.5f;
    float swordLifetime = 0.f;

    bool attackAnimation = true;
    bool swordCanSpawn = true;
    // sword end

    Rectangle src;
    Rectangle dest;
    Vector2 worldPos{};
};

#endif