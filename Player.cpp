#include "Player.h"

#include <string>

#include "raylib.h"
#include "raymath.h"

Player::Player(char* path, float _spawnX, float _spawnY) {
    texture = LoadTexture(path);
    width = texture.width / maxCols;
    height = texture.height / maxRows;

    spawnX = _spawnX - width * scale / 2;
    spawnY = _spawnY - height * scale;

    src = {0, 0, width, height};
    dest = {spawnX, spawnY, width * scale, height * scale};
    worldPos = {dest.x, dest.y};
}

void Player::render(float delta, Rectangle* platforms, int platformsSize, Sound jumpSound) {
    // handle movmement
    Vector2 direction{};
    if (IsKeyDown(KEY_A)) direction.x--;
    if (IsKeyDown(KEY_D)) direction.x++;

    velocityY += gravity * delta;
    spawnProtection -= delta;
    if (velocityY >= maxFallVel) velocityY = maxFallVel;

    direction = Vector2Normalize(direction);

    if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) && !isInAir) {
        PlaySound(jumpSound);
        velocityY = jumpPower;
        isInAir = true;
    }

    float playerFeet = getHitbox().y + getHitbox().height;
    isInAir = true;
    for (int i = 0; i < platformsSize && isInAir; i++) {
        Rectangle platform = platforms[i];
        if (CheckCollisionRecs(getHitbox(), platform) && (playerFeet - 20.f <= platform.y || playerFeet + 20.f <= platform.y)) {
            isInAir = false;
            if (velocityY >= 0) velocityY = 0;
        }
    }

    // handle animation
    runningTime += delta;
    if (direction.x < 0 && src.width > 0 || direction.x > 0 && src.width < 0) src.width *= -1;

    if (isInAir) {
        animate(jumpFrames, jumpRow);
    }
    if (Vector2Length(direction)) {
        worldPos = Vector2Add(worldPos, (Vector2Scale(direction, speed * delta)));
        if (!isInAir) animate(runFrames, runRow);

    } else
        animate(idleFrames, idleRow);

    worldPos.y += velocityY * delta;

    dest.x = worldPos.x;
    dest.y = worldPos.y;

    DrawTexturePro(texture, src, dest, Vector2{}, 0.f, WHITE);
}

bool Player::checkLava(Rectangle* lavas, int lavasSize) {
    Rectangle hitbox = getHitbox();
    for (int i = 0; i < lavasSize; i++)
        if (CheckCollisionRecs(hitbox, lavas[i])) return true;

    return false;
}

void Player::animate(float maxFrames, float row) {
    if (runningTime >= updateTime) {
        frame++;
        runningTime = 0;
        if (frame >= maxFrames) frame = 0;
        src.x = frame * width;
        src.y = height * row;
    }
}

Rectangle Player::getHitbox() {
    return {dest.x + paddingX, dest.y + paddingY, dest.width - paddingX * 2, dest.height - paddingY * 2};
}

int Player::updateHealth(int amount) {
    if (amount < 0 && spawnProtection > 0)
        return hearts;
    else if (amount < 0) {
        PlaySound(screamSound);
        spawnProtection = spawnProtectionMax;
    }

    hearts += amount;
    return hearts;
}

void Player::restartPosition() {
    worldPos = {spawnX, spawnY};
}

void Player::displayHearts(float delta) {
    float xPos = 20.f;
    float heartPadding = -50.f;
    for (int i = 0; i < hearts; i++) {
        heartAnim.render(delta, Vector2{xPos + heartPadding, 10.f});
        xPos += heartAnim.getWidth() + heartPadding;
    }
}

void Player::setStage(Stage stage) {
    if (stage.stageId == 0) {
        texture = LoadTexture("assets/images/orange.png");
        speed = baseSpeed;
        jumpPower = baseJumpPower;
        canAttack = false;
    } else if (stage.stageId == 1) {
        texture = LoadTexture("assets/images/second-coming.png");
        updateHealth(2);
        speed += 50;
        jumpPower += 100;
        canAttack = false;
    } else if (stage.stageId == 2) {
        texture = LoadTexture("assets/images/ultra-coming.png");
        updateHealth(2);
        canAttack = true;
    }
}