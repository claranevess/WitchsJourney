#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    ENEMY_DOWN,
    ENEMY_UP,
    ENEMY_LEFT,
    ENEMY_RIGHT,
} EnemyDirection;

typedef struct Enemy {
    Vector2 position;
    float speed;
    Texture2D texture;
    Rectangle frameRec;
    int currentFrame;
    int frameCounter;
    int frameSpeed;
    int maxFrames;
    bool active;
    float scale;
    float health;
    Rectangle hitbox;
    int tipo;
    int maxHealth;
} Enemy;

void InitEnemy(Enemy* enemy, int tipo);
void UpdateEnemy(Enemy* enemy, Vector2 targetPosition, bool coraAlive);
void DrawEnemy(Enemy* enemy);
void UnloadEnemy(Enemy* enemy);

#endif
