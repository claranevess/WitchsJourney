#include "cora.h"

Cora initCora(void) {
    Cora cora = { 0 };
    cora.health = 100;
    cora.position = (Vector2){100, 100};
    cora.speed = (Vector2){2.5f, 2.5f};
    cora.direction = DIR_IDLE;

    // Carregando texturas de spritesheet
    cora.textures[DIR_DOWN]  = LoadTexture("resources/cora_down.png");
    cora.textures[DIR_UP]    = LoadTexture("resources/cora_up.png");
    cora.textures[DIR_LEFT]  = LoadTexture("resources/cora_left.png");
    cora.textures[DIR_RIGHT] = LoadTexture("resources/cora_right.png");
    cora.textures[DIR_IDLE]  = LoadTexture("resources/player.png");

    // Definindo quantidade de frames de cada animação
    cora.frames[DIR_DOWN]  = 4;
    cora.frames[DIR_UP]    = 4;
    cora.frames[DIR_LEFT]  = 6;
    cora.frames[DIR_RIGHT] = 6;
    cora.frames[DIR_IDLE]  = 2;

    // Inicialização do controle de animação
    cora.frameSpeed = 8;
    cora.frameCounter = 0;
    cora.currentFrame = 0;

    // Inicializa o frameRec com base na textura atual
    int maxFrames = cora.frames[cora.direction];
    Texture2D current = cora.textures[cora.direction];
    cora.frameRec = (Rectangle){ 0, 0, (float)(current.width / maxFrames), (float)current.height };

    return cora;
}

void updateCora(Cora *cora) {
    Vector2 input = { 0 };

    if (IsKeyDown(KEY_RIGHT)) {
        input.x += 1;
        cora->direction = DIR_RIGHT;
    } else if (IsKeyDown(KEY_LEFT)) {
        input.x -= 1;
        cora->direction = DIR_LEFT;
    } else if (IsKeyDown(KEY_UP)) {
        input.y -= 1;
        cora->direction = DIR_UP;
    } else if (IsKeyDown(KEY_DOWN)) {
        input.y += 1;
        cora->direction = DIR_DOWN;
    } else {
        cora->direction = DIR_IDLE;
    }

    // Atualiza posição
    cora->position.x += input.x * cora->speed.x;
    cora->position.y += input.y * cora->speed.y;

    // Atualiza animação
    cora->frameCounter++;
    if (cora->frameCounter >= (60 / cora->frameSpeed)) {
        cora->frameCounter = 0;
        cora->currentFrame++;

        if (cora->currentFrame >= cora->frames[cora->direction]) {
            cora->currentFrame = 0;
        }
    }

    // Atualiza recorte da animação
    Texture2D current = cora->textures[cora->direction];
    int maxFrames = cora->frames[cora->direction];
    cora->frameRec = (Rectangle){
        cora->currentFrame * (float)(current.width / maxFrames),
        0,
        (float)(current.width / maxFrames),
        (float)current.height
    };
}

void drawCora(Cora *cora) {
    DrawTextureRec(
        cora->textures[cora->direction],
        cora->frameRec,
        cora->position,
        WHITE
    );
}

void unloadCora(Cora *cora) {
    for (int i = 0; i < 5; i++) {
        UnloadTexture(cora->textures[i]);
    }
}
