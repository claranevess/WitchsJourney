#ifndef PROJETIL_H
#define PROJETIL_H

#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include "cora.h"

typedef struct Node {
    int valor; // valor indo de 1 a 4 e compondo os tipos dos ataques, 1 = Agua; 2 = Terra; 3 = Vento; 4 = Fogo;
    struct Node* prox;
    struct Node* ant;
} Node;                                 

typedef struct Projectile {
    Vector2 position;
    Vector2 direction;
    float speed;
    bool active;
    Texture2D textures[4];
    Rectangle frameRec;
    float scale;
    Node* tipo;
    // Animação
    int currentFrame;
    int maxFrames;
    float frameSpeed;
    float frameCounter;
} Projectile;

const char* GetAttackName(int valor);
Node* CreateAttackTypes();
void InitProjectile(Projectile* p, Texture2D textures[4]);
void ShootProjectile(Projectile* p, Cora* cora);
void UpdateProjectile(Projectile* p);
void DrawProjectile(Projectile* p);
float GetDamageMultiplier(int tipoAtaque, int tipoDefesa);

#endif
