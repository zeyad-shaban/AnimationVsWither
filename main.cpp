#include <string>

#include "AnimImg.h"
#include "Player.h"
#include "Stage.h"
#include "WitherSkeleton.h"
#include "raylib.h"

int factorial(int num) {
    // the most random function to ever exist in existence btw
    if (num == 0) return 1;
    return num * factorial(num - 1);
}

int main() {
    float winWidth = 1200;
    float winHeight = 720;
    const int platformsLen = 4;
    const int lavasSize = 1;
    const int witherSkeletonsSize = 3;
    const int stagesSize = 3;
    InitWindow(winWidth, winHeight, "Animation vs Wither");
    InitAudioDevice();

    float surviveTime = 0.f;
    Stage stages[] = {
        {0, 0.f, 25.f},
        {1, 25.f, 40.f},
        {2, 40.f, 60.f},
    };

    Rectangle platforms[platformsLen] = {
        {(winWidth + 24) / 2.f, 510, 152, 50},
        {winWidth - 327, winHeight - 5, 152, 50},
        {310, winHeight - 5, 152, 50},
        {360.66f, 355, 101.333f, 50},
    };

    Rectangle lavas[lavasSize] = {
        {-winWidth * 2, winHeight + 150, winWidth * 10, winHeight},
    };

    Player orange("assets/images/orange.png", platforms[0].x + platforms[0].width / 2, platforms[0].y - 500);
    Sound deathSound = LoadSound("assets/sounds/death.mp3");
    Texture2D map = LoadTexture("./assets/images/map.png");
    float mapScale = 1.6f;

    WitherSkeleton witherSkeletons[witherSkeletonsSize] = {
        {"assets/images/wither-skeleton.png", 150, map.height * mapScale - 450, -1},
        {"assets/images/wither-skeleton.png", 180, 470, -1},
        {"assets/images/wither-skeleton.png", map.width * mapScale - 100, map.height * mapScale - 350, 1},
    };

    Texture2D scotland = LoadTexture("assets/images/scotland.png");
    AnimImg toothless("assets/images/toothless.png", 6, 5.f, 18.f);

    Sound jumpSound = LoadSound("assets/sounds/jump.wav");
    Sound winSound = LoadSound("assets/sounds/win.mp3");
    Camera2D camera{{winWidth / 2.f, winHeight / 2.f}, orange.getWorldPos(), 0, 1.f};

    float restartWait = 0.f;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);

        float delta = GetFrameTime();
        if (delta > 2) {
            EndMode2D();
            EndDrawing();
            continue;
        }

        DrawTextureEx(map, {0, 0}, 0, mapScale, WHITE);

        if (!orange.updateHealth(0) && restartWait <= 5.f) {
            if (!restartWait) PlaySound(deathSound);
            restartWait += delta;
            EndMode2D();

            DrawTextureEx(scotland, {(winWidth - scotland.width * 5.f) / 2.f, (winHeight - scotland.height * 5.f) / 2.f}, 0.f, 5.f, WHITE);
            EndDrawing();
            continue;
        } else if (surviveTime >= stages[stagesSize - 1].endTime && restartWait <= 18.f) {
            if (!restartWait) PlaySound(winSound);
            restartWait += delta;
            EndMode2D();

            toothless.render(delta, {0, 0});
            EndDrawing();
            continue;
        } else if (!orange.updateHealth(0)) {
            orange.updateHealth(2);
            orange.restartPosition();
            surviveTime = 0.f;
            restartWait = 0.f;
            for (int i = 0; i < stagesSize; i++) {
                stages[i].didFirstStart = false;
            }
        }

        surviveTime += delta > 5 ? 0 : delta;
        for (int i = 0; i < stagesSize; i++) {
            if (surviveTime >= stages[i].startTime && surviveTime <= stages[i].endTime && !stages[i].didFirstStart) {
                orange.setStage(stages[i]);
                for (int j = 0; j < witherSkeletonsSize; j++) witherSkeletons[j].setStage(stages[i]);
                stages[i].didFirstStart = true;
            }
        }

        // TODO spawn wither skeletons
        orange.render(delta, platforms, platformsLen, jumpSound);

        camera.target = orange.getWorldPos();
        float camMaxRight = map.width * mapScale - winWidth / 2.f;
        float camMaxLeft = winWidth / 2.f;
        float camMaxTop = winHeight / 2.f;
        float camMaxBottom = map.height * mapScale - winHeight / 2.f;

        if (camera.target.x >= camMaxRight) camera.target.x = camMaxRight;
        if (camera.target.x <= camMaxLeft) camera.target.x = camMaxLeft;  // hitmaxright
        if (camera.target.y >= camMaxBottom) camera.target.y = camMaxBottom;
        if (camera.target.y <= camMaxTop) camera.target.y = camMaxTop;

        // draw wither skeletons
        for (int i = 0; i < witherSkeletonsSize; i++) {
            witherSkeletons[i].render(delta, &orange);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (orange.getCanAttack()) {
                Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
                Vector2 orangeHead = {(orange.getWorldPos().x + orange.getWidth() / 2.f) + 20.f, orange.getWorldPos().y + 20};
                DrawLineV(orangeHead, mousePos, GREEN);
                DrawLineV({(orange.getWorldPos().x + orange.getWidth() / 2.f) + 40.f, orange.getWorldPos().y + 30}, mousePos, GREEN);

                for (int i = 0; i < witherSkeletonsSize; i++)
                    witherSkeletons[i].hitSword(orangeHead, mousePos);
            }
        };

        EndMode2D();

        bool isOnLava = orange.checkLava(lavas, lavasSize);
        if (isOnLava) {
            orange.updateHealth(-1);
            orange.restartPosition();
        }

        orange.displayHearts(delta);
        DrawText(std::to_string((int)surviveTime).c_str(), winWidth / 2, 10, 50, WHITE);

        if (IsKeyPressed(50)) orange.updateHealth(factorial(2));
        if (IsKeyPressed(51)) orange.updateHealth(factorial(3));

        EndDrawing();
    }

    UnloadSound(jumpSound);
    CloseAudioDevice();
    CloseWindow();
}

/*TODO

hmm lets see i need to make wither skeletons shoot their swords at the player

then make a little cutscene of them capcturing him

then me you evo jump in to rescue him

wither skeletons gets angry and turn to wither

herobrine joins the chat

wither gets defeated and turns to wither storm

king joins the chat

me do some hoobla woopla to destroy the wither storm and accidnetly make a time machien that destroys the game

*/

// TODO aniamte fall and jump better
// TODO touch lava to lose hearts