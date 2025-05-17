#include "cora.h"
#include "enemy.h"

Cora initCora(void) {
    Cora cora = { 0 };
    cora.isAlive = true;
    cora.health = 100;
    cora.position = (Vector2){ 640, 360 };
    cora.speed = (Vector2){ 2.5f, 2.5f };
    cora.direction = DIR_IDLE;

    // Carregando texturas de spritesheet
    cora.textures[DIR_DOWN] = LoadTexture("resources/assets/baixo.png");
    cora.textures[DIR_UP] = LoadTexture("resources/assets/cima.png");
    cora.textures[DIR_LEFT] = LoadTexture("resources/assets/esquerda.png");
    cora.textures[DIR_RIGHT] = LoadTexture("resources/assets/direita.png");
    cora.textures[DIR_IDLE] = LoadTexture("resources/assets/idle.png");
	cora.textures[DIR_DYING] = LoadTexture("resources/assets/Dying_KG_1.png");

    // Definindo quantidade de frames de cada anima��o
    cora.frames[DIR_DOWN] = 4;
    cora.frames[DIR_UP] = 4;
    cora.frames[DIR_LEFT] = 7;
    cora.frames[DIR_RIGHT] = 7;
    cora.frames[DIR_IDLE] = 4;
	cora.frames[DIR_DYING] = 5;

    // Inicializa��o do controle de anima��o
    cora.frameSpeed = 8;
    cora.frameCounter = 0;
    cora.currentFrame = 0;

    // Inicializa o frameRec com base na textura atual
    int maxFrames = cora.frames[cora.direction];
    Texture2D current = cora.textures[cora.direction];
    cora.frameRec = (Rectangle){ 0, 0, (float)(current.width / maxFrames), (float)current.height };

    // Define hitbox com base no tamanho do primeiro sprite carregado
    cora.hitbox = (Rectangle){
        cora.position.x,
        cora.position.y,
        cora.frameRec.width,
        cora.frameRec.height
    };

    return cora;
}

void updateCora(Cora* cora) {
    Vector2 input = { 0 };

    // limites do mapa
    const float minX = 0.0f;
    const float minY = 0.0f;
    const float maxX = 1280.0f - cora->frameRec.width;   // largura da janela � largura do sprite
    const float maxY = 720.0f  - cora->frameRec.height;  // altura  da janela � altura  do sprite

	// Verifica se Cora est� dentro dos limites do mapa
    if (cora->position.x < minX) cora->position.x = minX;
    if (cora->position.x > maxX) cora->position.x = maxX;
    if (cora->position.y < minY) cora->position.y = minY;
    if (cora->position.y > maxY) cora->position.y = maxY;

	// verifica se Cora t� viva
    if (!cora->isAlive) {
        // se n�o estiver, reproduz os frames da morte
        cora->frameCounter++;
        if (cora->frameCounter >= (60 / cora->frameSpeed)) {
            cora->frameCounter = 0;
            cora->currentFrame++;

            if (cora->currentFrame >= cora->frames[DIR_DYING]) {
                cora->currentFrame = cora->frames[DIR_DYING] - 1; // Para no �ltimo frame
            }
        }

        Texture2D dyingTexture = cora->textures[DIR_DYING];
        int maxFrames = cora->frames[DIR_DYING];

        cora->frameRec = (Rectangle){
            cora->currentFrame * (float)(dyingTexture.width / maxFrames),
            0,
            (float)(dyingTexture.width / maxFrames),
            (float)dyingTexture.height
        };

        return; // ?? N�o atualiza posi��o nem hitbox se estiver morta
    }


    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        input.x += 3;
        cora->direction = DIR_RIGHT;
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        input.x -= 3;
        cora->direction = DIR_LEFT;
    }
    else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        input.y -= 3;
        cora->direction = DIR_UP;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        input.y += 3;
        cora->direction = DIR_DOWN;
    }
    else {
        cora->direction = DIR_IDLE;
    }

    // Atualiza posi��o
    cora->position.x += input.x * cora->speed.x;
    cora->position.y += input.y * cora->speed.y;

    // Atualiza anima��o
    cora->frameCounter++;
    if (cora->frameCounter >= (60 / cora->frameSpeed)) {
        cora->frameCounter = 0;
        cora->currentFrame++;

        if (cora->currentFrame >= cora->frames[cora->direction]) {
            cora->currentFrame = 0;
        }
    }

    // Atualiza recorte da anima��o
    Texture2D current = cora->textures[cora->direction];
    int maxFrames = cora->frames[cora->direction];
    cora->frameRec = (Rectangle){
        cora->currentFrame * (float)(current.width / maxFrames),
        0,
        (float)(current.width / maxFrames),
        (float)current.height
    };

    // Atualiza hitbox com a nova posi��o
     cora->hitbox = (Rectangle){
     cora->position.x + 30,
     cora->position.y,
     cora->frameRec.width - 60,
     cora->frameRec.height
    };
}

void drawCora(Cora* cora) {
    Texture2D textureToDraw;

    // visualiza��o da Cora dependendo do estado dela, entrando no else se health chegar a 0, e textura indo pra a de dying
    if (cora->isAlive) {
        textureToDraw = cora->textures[cora->direction];
    }
    else {
        textureToDraw = cora->textures[DIR_DYING];
    }

    DrawTextureRec(
        textureToDraw,
        cora->frameRec,
        cora->position,
        WHITE
    );


    DrawText(TextFormat("HP: %d", cora->health), cora->position.x, cora->position.y - 20, 20, RED);
}

void unloadCora(Cora* cora) {
    for (int i = 0; i < 5; i++) {
        UnloadTexture(cora->textures[i]);
    }
}

void checkCoraCollision(Cora* cora, Enemy* enemies, int enemyCount) {
    if (!cora->isAlive) return;

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].active) {
            Rectangle enemyHitbox = {
                enemies[i].position.x,
                enemies[i].position.y,
                enemies[i].frameRec.width * enemies[i].scale,
                enemies[i].frameRec.height * enemies[i].scale
            };

            if (CheckCollisionRecs(cora->hitbox, enemyHitbox)) {
                cora->health -= 10;
                enemies[i].active = false; // opcional: "desativa" o inimigo que bateu
                if (cora->health <= 0) {
                    cora->isAlive = false;
                }
            }
        }
    }
}
