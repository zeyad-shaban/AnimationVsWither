#include "WitherSkeleton.h"

#include <string>

#include "Player.h"
#include "raylib.h"
#include "raymath.h"

WitherSkeleton::WitherSkeleton(char* path, float _spawnX, float _spawnY, int flip) {
    texture = LoadTexture(path);
    width = texture.width / maxCols;
    height = texture.height / maxRows;

    spawn.x = _spawnX - width * scale / 2;
    spawn.y = _spawnY - height * scale;

    src = {0, 0, width * flip, height};
    dest = {spawn.x, spawn.y, width * scale, height * scale};
    worldPos = {dest.x, dest.y};
}

void WitherSkeleton::render(float delta, Player* target) {
    runningTime += delta;

    if (attackAnimation) {
        attackAnimation = !animate(attackFrames, attackRow, false);
    } else if (swordCanSpawn) {
        animate(idleFrames, idleRow, true);
        bool hitPlayer = throwSword(delta, target);
        if (hitPlayer) {
            target->updateHealth(-1);
            swordLifetime = 999;
        }
    } else {
        frame = -1;
        attackAnimation = true;
        swordCanSpawn = true;
    }

    DrawTexturePro(texture, src, dest, Vector2{}, 0.f, WHITE);
}

bool WitherSkeleton::animate(float maxFrames, float row, bool loop) {
    if (runningTime >= updateTime) {
        frame++;
        runningTime = 0;
        if (frame >= maxFrames) {
            frame = -1;
            if (!loop) return true;
        }
        src.x = frame * width;
        src.y = height * row;
    }
    return false;
}

Rectangle WitherSkeleton::getHitbox() {
    return {dest.x + padding.x, dest.y + padding.y, dest.width - padding.x * 2, dest.height - padding.y * 2};
}

bool WitherSkeleton::throwSword(float delta, Player* target) {
    // sword lifetime
    swordLifetime += delta;
    if (swordLifetime >= swordMaxLifetime) {
        swordLifetime = 0;
        swordSpawned = false;
        swordCanSpawn = false;
    }

    // animate
    swordRunningTime += delta;

    if (swordRunningTime >= swordUpdateTime) {
        swordRunningTime = 0;
        swordFrame++;

        if (swordFrame >= swordCols) swordFrame = 0;
    }

    // move sword
    Rectangle swordSrc = {frame * (sword.width / swordCols), 0, sword.width / swordCols, static_cast<float>(sword.height)};
    swordDest = {0, 0, sword.width / swordCols * swordScale, sword.height * swordScale};

    if (!swordSpawned) {
        swordPos = worldPos;
        swordSpawned = true;
    } else {
        Vector2 direction = Vector2Normalize(Vector2Subtract(target->getWorldPos(), swordPos));
        swordPos = Vector2Add(swordPos, Vector2Scale(direction, swordSpeed));
    }

    swordDest.x = swordPos.x;
    swordDest.y = swordPos.y;
    DrawTexturePro(sword, swordSrc, swordDest, Vector2{}, 0.f, WHITE);

    return CheckCollisionRecs(target->getHitbox(), swordDest);
}

void WitherSkeleton::setStage(Stage stage) {
    if (stage.stageId == 0) {
        swordSpeed = 3.5f;
        updateTime = 1 / 30.f;
    } else if (stage.stageId == 1) {
        swordSpeed = 6.f;
        updateTime = 1 / 40.f;
    } else if (stage.stageId == 2) {
        swordSpeed = 15.f;
        updateTime = 1 / 100.f;
    }
}

void WitherSkeleton::hitSword(Vector2 p1, Vector2 p2) {
    if (doesRectIntersectLine(swordDest, p1, p2))
        swordLifetime = swordMaxLifetime;
}

// for marking purposes, the two functions below were made by chatgpt, i'm not good at math like him/her/it :(
bool WitherSkeleton::doLinesIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    float denominator = ((d.y - c.y) * (b.x - a.x)) - ((d.x - c.x) * (b.y - a.y));
    float numerator1 = ((d.x - c.x) * (a.y - c.y)) - ((d.y - c.y) * (a.x - c.x));
    float numerator2 = ((b.x - a.x) * (a.y - c.y)) - ((b.y - a.y) * (a.x - c.x));

    if (denominator == 0 && numerator1 == 0 && numerator2 == 0)
        return true;

    return (denominator != 0) && (numerator1 / denominator >= 0) && (numerator1 / denominator <= 1) && (numerator2 / denominator >= 0) && (numerator2 / denominator <= 1);
}

bool WitherSkeleton::doesRectIntersectLine(Rectangle rect, Vector2 lineStart, Vector2 lineEnd) {
    Vector2 rectPoints[4] = {
        {rect.x, rect.y},
        {rect.x + rect.width, rect.y},
        {rect.x + rect.width, rect.y + rect.height},
        {rect.x, rect.y + rect.height}};

    for (int i = 0; i < 4; ++i) {
        int j = (i + 1) % 4;

        if (doLinesIntersect(rectPoints[i], rectPoints[j], lineStart, lineEnd))
            return true;
    }

    return false;
}