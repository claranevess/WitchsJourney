#include "projetil.h"
#include "raymath.h" // Para Vector2Normalize
#include <stdlib.h>


Node* CreateAttackTypes() {
    Node* agua = (Node*)malloc(sizeof(Node));
    Node* terra = (Node*)malloc(sizeof(Node));
    Node* vento = (Node*)malloc(sizeof(Node));
    Node* fogo = (Node*)malloc(sizeof(Node));

    agua->valor = 1;
    terra->valor = 2;
    vento->valor = 3;
    fogo->valor = 4;

    // Encadeamento circular
    agua->prox = terra;
    terra->prox = vento;
    vento->prox = fogo;
    fogo->prox = agua;

    agua->ant = fogo;
    terra->ant = agua;
    vento->ant = terra;
    fogo->ant = vento;

    return agua; // retorna o primeiro tipo
}


void InitProjectile(Projectile* p, Texture2D texture) {
    p->texture = texture;
    p->position = (Vector2){ 0, 0 };
    p->direction = (Vector2){ 0, 0 };
    p->speed = 8.0f;
    p->active = false;
    p->scale = 2.0f;
    p->tipo = CreateAttackTypes();

    p->frameRec = (Rectangle){
        0, 0,
        (float)texture.width,
        (float)texture.height
    };
}

void ShootProjectile(Projectile* p, Cora* cora) {
    if (!p->active) {
        p->active = true;

        // Origem: centro da Cora
        p->position = (Vector2){
            cora->position.x + cora->frameRec.width / 2,
            cora->position.y + cora->frameRec.height / 2
        };

        Vector2 mousePos = GetMousePosition();
        p->direction = Vector2Subtract(mousePos, p->position);
        p->direction = Vector2Normalize(p->direction);
    }
}

void UpdateProjectile(Projectile* p) {
    if (p->active) {
        p->position.x += p->direction.x * p->speed;
        p->position.y += p->direction.y * p->speed;

        // Verifica se saiu da tela
        if (p->position.x < 0 || p->position.x > GetScreenWidth() ||
            p->position.y < 0 || p->position.y > GetScreenHeight()) {
            p->active = false;
        }

        if (IsKeyPressed(KEY_C)) {
            p->tipo = p->tipo->prox;
        }
        else if (IsKeyPressed(KEY_X)) {
            p->tipo = p->tipo->ant;
        }

    }
}

void DrawProjectile(Projectile* p) {
    if (p->active) {
        DrawTextureEx(
            p->texture,
            p->position,
            0.0f,
            p->scale,
            WHITE
        );
    }
    DrawText(GetAttackName(p->tipo->valor), p->position.x, p->position.y - 20, 20, WHITE);
}


const char* GetAttackName(int valor) {
    switch (valor) {
    case 1: return "Água";
    case 2: return "Terra";
    case 3: return "Vento";
    case 4: return "Fogo";
    default: return "Desconhecido";
    }
}