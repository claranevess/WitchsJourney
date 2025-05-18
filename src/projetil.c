#include "projetil.h"
#include "raymath.h" // Para Vector2Normalize
#include <stdlib.h>

float GetDamageMultiplier(int tipoAtaque, int tipoDefesa) {
    float tabela[4][4] = {
        {0.0f, 1.0f, 2.0f, 0.5f},
        {1.0f, 0.0f, 0.5f, 2.0f},
        {0.5f, 2.0f, 0.0f, 1.0f},
        {2.0f, 0.5f, 1.0f, 0.0f}
    };

    if (tipoAtaque < 1 || tipoAtaque > 4 || tipoDefesa < 1 || tipoDefesa > 4) {
        return 1.0f;// Dano no Boss
    }

    int col = tipoAtaque - 1;
    int row = tipoDefesa - 1;
    float multiplier = tabela[row][col];
    printf("DEBUG (DENTRO DA FUNÇÃO): resultado = %.2f\n", multiplier);
    return multiplier; 
}

Node* CreateAttackTypes() {
    Node* agua = (Node*)malloc(sizeof(Node));
    Node* terra = (Node*)malloc(sizeof(Node));
    Node* vento = (Node*)malloc(sizeof(Node));
    Node* fogo = (Node*)malloc(sizeof(Node));

    agua->valor = 1;
    terra->valor = 2;
    vento->valor = 3;
    fogo->valor = 4;

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


void InitProjectile(Projectile* p, Texture2D textures[4]) {
    for (int i = 0; i < 4; i++) {
        p->textures[i] = textures[i];
    }

    p->position = (Vector2){ 0, 0 };
    p->direction = (Vector2){ 0, 0 };
    p->speed = 8.0f;
    p->active = false;
    p->scale = 1.2f;
    p->tipo = CreateAttackTypes();

    p->maxFrames = 5;
    p->currentFrame = 0;
    p->frameSpeed = 8.0f;
    p->frameCounter = 0;

    // Assuma que todas as texturas t�m as mesmas dimens�es
    p->frameRec = (Rectangle){
        0, 0,
        (float)p->textures[0].width / p->maxFrames,
        (float)p->textures[0].height
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

        // Atualiza��o de anima��o
        p->frameCounter += GetFrameTime() * p->frameSpeed;
        if (p->frameCounter >= 1) {
            p->currentFrame++;
            if (p->currentFrame >= p->maxFrames) {
                p->currentFrame = 0;
            }
            p->frameRec.x = (float)p->currentFrame * p->frameRec.width;
            p->frameCounter = 0;
        }
    }
    if (IsKeyPressed(KEY_C)) {
        p->tipo = p->tipo->prox;
    }
    else if (IsKeyPressed(KEY_X)) {
        p->tipo = p->tipo->ant;
    }
}

void DrawProjectile(Projectile* p) {
    if (!p->active) return;

    int index = p->tipo->valor - 1;
    Texture2D tex = p->textures[index];

    Rectangle dest = {
        p->position.x,
        p->position.y,
        p->frameRec.width * p->scale,
        p->frameRec.height * p->scale
    };

    Vector2 origin = { dest.width / 2, dest.height / 2 };
    float angle = atan2f(p->direction.y, p->direction.x) * RAD2DEG;

    DrawTexturePro(tex, p->frameRec, dest, origin, angle, WHITE);
    //DrawText(GetAttackName(p->tipo->valor), p->position.x - 20, p->position.y - 40, 20, WHITE);
}


const char* GetAttackName(int valor) {
    switch (valor) {
    case 1: return "�gua";
    case 2: return "Terra";
    case 3: return "Vento";
    case 4: return "Fogo";
    default: return "Desconhecido";
    }
}