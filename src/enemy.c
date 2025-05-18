#include "enemy.h"
#include "cora.h"
#include <math.h>

void InitEnemy(Enemy* enemy, int tipo) {
    enemy->tipo = tipo;
    enemy->scale = 2.0f;
    enemy->active = true;

    switch (tipo) {
        case 1:
            enemy->texture = LoadTexture("resources/assets/enemy.png");
            enemy->health = 2;
            enemy->maxHealth = enemy->health;
            enemy->speed = 1.5f;
            enemy->maxFrames = 7;
            break;
        case 2:
            enemy->texture = LoadTexture("resources/assets/morcego.png");
            enemy->health = 1;
            enemy->maxHealth = enemy->health;
            enemy->speed = 2.0f;
            enemy->maxFrames = 8;
            break;
        case 3:
            enemy->texture = LoadTexture("resources/assets/boss.png");
            enemy->health = 5;
            enemy->maxHealth = enemy->health;
            enemy->speed = 0.8f;
            enemy->maxFrames = 4;
            break;
        case 4:
            enemy->texture = LoadTexture("resources/assets/goblin.png");
            enemy->health = 3;
            enemy->maxHealth = enemy->health;
            enemy->speed = 1.2f;
            enemy->maxHealth = enemy->health;
            enemy->maxFrames = 6;
            break;
        case 5: // BOSS
            enemy->texture = LoadTexture("resources/assets/boss.png");
            enemy->health = 15;
            enemy->maxHealth = enemy->health;
            enemy->speed = 1.0f;
            enemy->maxFrames = 4;
            break;

    }

    enemy->frameSpeed = 10;
    enemy->currentFrame = 0;
    enemy->frameCounter = 0;
    enemy->frameRec = (Rectangle){ 0, 0, (float)(enemy->texture.width / enemy->maxFrames), (float)enemy->texture.height };

    // Posição inicial igual a antes
    int spawnSide = GetRandomValue(0, 3);
    switch (spawnSide) {
        case 0:
            enemy->position = (Vector2){ -enemy->texture.width, GetRandomValue(0, 720 - enemy->texture.height)};
            break;
        case 1:
            enemy->position = (Vector2){ 1280, GetRandomValue(0, 720 - enemy->texture.height)};
            break;
        case 2:
            enemy->position = (Vector2){ GetRandomValue(0, 1280 - enemy->texture.width), -enemy->texture.height };
            break;
        case 3:
            enemy->position = (Vector2){ GetRandomValue(0, 1280 - enemy->texture.width), 720 };
            break;
    }
}

void UpdateEnemy(Enemy* enemy, Vector2 targetPosition, bool coraAlive) {
    if (!enemy->active) return;
	if (!coraAlive) return;

    // Movimento em direcao a Cora
    Vector2 direction = {
        targetPosition.x - enemy->position.x,
        targetPosition.y - enemy->position.y
    };

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction.x /= length;
        direction.y /= length;
    }

    enemy->position.x += direction.x * enemy->speed;
    enemy->position.y += direction.y * enemy->speed;

    // Animacao
    enemy->frameCounter++;
    if (enemy->frameCounter >= (60 / enemy->frameSpeed)) {
        enemy->frameCounter = 0;
        enemy->currentFrame++;

        if (enemy->currentFrame >= enemy->maxFrames) {
            enemy->currentFrame = 0;
        }

        enemy->frameRec.x = enemy->currentFrame * enemy->frameRec.width;
    }

    // att hitbox
    enemy->hitbox = (Rectangle){
    enemy->position.x + 48,
    enemy->position.y + 65,
    enemy->frameRec.width - 15,
    enemy->frameRec.height,
    };

}

void DrawEnemy(Enemy* enemy) {
    if (!enemy->active) return;

    Rectangle source = enemy->frameRec;

    Rectangle dest = {
        enemy->position.x,
        enemy->position.y,
        source.width * enemy->scale,
        source.height * enemy->scale
    };

    Vector2 origin = { 0, 0 }; // desenha a partir do canto superior esquerdo

    DrawTexturePro(enemy->texture, source, dest, origin, 0.0f, WHITE);
}

void DrawEnemyHealthBar(Enemy* enemy) {
    if (!enemy->active) return;

    float barWidth = 50.0f;
    float barHeight = 6.0f;
    float spacing = 10.0f;

    // Calcula a posição da barra (acima do inimigo)
    float x = enemy->position.x + (enemy->frameRec.width * enemy->scale / 2) - barWidth / 2;
    float y = enemy->position.y - spacing;

    // Percentual de vida
    float healthPercent = (float)enemy->health / enemy->maxHealth;

    // Fundo da barra (vermelho)
    DrawRectangle(x, y, barWidth, barHeight, RED);

    // Parte preenchida (verde)
    DrawRectangle(x, y, barWidth * healthPercent, barHeight, GREEN);

    // Contorno da barra
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
}

void UnloadEnemy(Enemy* enemy) {
    UnloadTexture(enemy->texture);
}
