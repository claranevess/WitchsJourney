#include "raylib.h"
#include "cora.h"
#include "enemy.h"
#include "tools.h"
#include "projetil.h"

extern const int screenWidth;
extern const int screenHeight;
Texture2D aguaIcon, terraIcon, ventoIcon, fogoIcon, magicTex, background1;

void DrawAttackInventory(Node* tipoAtual, float screenW, float screenH) {
    float startX = screenW / 2 - 100;
    float y = screenH - 80;
    float size = 48;
    float spacing = 10;

    Texture2D icons[4] = { aguaIcon, terraIcon, ventoIcon, fogoIcon };
    
    for (int i = 0; i < 4; i++) {
        float x = startX + i * (size + spacing);

        if (tipoAtual->valor == i + 1) {
            DrawRectangleLines(x - 2, y - 2, size + 4, size + 4, YELLOW);
        }

        DrawTextureEx(icons[i], (Vector2){ x, y }, 0.0f, size / icons[i].width, WHITE);
    }
}

void map1(void) {
    bool showHitboxes = false;
    InitWindow(screenWidth, screenHeight, "Mapa 1");
    InitAudioDevice(); // se usar sons
    SetTargetFPS(60);

    background1 = LoadTexture("resources/assets/background1.png");
    magicTex = LoadTexture("resources/assets/projetil.png");
    aguaIcon = LoadTexture("resources/assets/icons/agua_icon.png");
    terraIcon = LoadTexture("resources/assets/icons/terra_icon.png");
    ventoIcon = LoadTexture("resources/assets/icons/vento_icon.png");
    fogoIcon = LoadTexture("resources/assets/icons/fogo_icon.png"); 
    Cora cora = initCora();
    

    // Cria o proj�til e deixa inativo
    Projectile projectileW;
    InitProjectile(&projectileW, magicTex);

    #define MAX_ENEMIES 10

    // Criando e inicializando v�rios inimigos
    Enemy enemies[MAX_ENEMIES];
    for (int i = 0; i < MAX_ENEMIES; i++) {
        InitEnemy(&enemies[i]);
    }


    while (!WindowShouldClose()) {
        updateCora(&cora);
        if (IsKeyPressed(KEY_H)) {
            showHitboxes = !showHitboxes;
        }

        // Atualize inimigos e fa�a a colis�o AQUI, dentro do loop while
        for (int i = 0; i < MAX_ENEMIES; i++) {
            UpdateEnemy(&enemies[i], cora.position, cora.isAlive);

            // Atualiza a hitbox do inimigo dentro do UpdateEnemy (j� deve estar feito)

            // Colis�o e dano
            if (enemies[i].active && cora.isAlive &&
                CheckCollisionRecs(cora.hitbox, enemies[i].hitbox)) {

                cora.health -= 1;
                if (cora.health <= 0) {
                    cora.health = 0;
                    cora.isAlive = false;
                }
            }
        }        

        if (projectileW.active) {
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active &&
                    CheckCollisionCircleRec(projectileW.position, projectileW.frameRec.width * projectileW.scale * 0.5f,
                        enemies[i].hitbox)) {

                    enemies[i].health -= 1;  // dano
                    projectileW.active = false;

                    if (enemies[i].health <= 0) {
                        enemies[i].active = false;
                    }
                    break;
                }
            }
        }

        // Dispara quando clicar com o bot�o esquerdo do mouse
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cora.isAlive) {
            ShootProjectile(&projectileW, &cora);
        }

        // Atualiza a posi��o do proj�til (mesmo se inativo faz nada)
        UpdateProjectile(&projectileW);

        BeginDrawing();
        DrawTexture(background1, 0, 0, WHITE);
        DrawAttackInventory(projectileW.tipo, screenWidth, screenHeight);
        drawCora(&cora);

        DrawProjectile(&projectileW);

        // Desenhar cada inimigo
        for (int i = 0; i < MAX_ENEMIES; i++) {
            DrawEnemy(&enemies[i]);
            if (enemies[i].active) {
                // Desenha a vida acima do inimigo
                DrawText(TextFormat("%d", enemies[i].health),
                    enemies[i].position.x + 75,
                    enemies[i].position.y + 40,
                    20,
                    WHITE);
            }
        }

        DrawDebugHitboxes(&cora, enemies, MAX_ENEMIES, showHitboxes);

        // *** Aqui que voc� coloca a exibi��o da vida e da mensagem ***
        DrawText(TextFormat("Vida: %d", cora.health), 10, 10, 20, WHITE);

        if (!cora.isAlive) {
            DrawText("Cora morreu!", screenWidth / 2 - 80, screenHeight / 2, 30, WHITE);
        }

        EndDrawing();
    }

    unloadCora(&cora);

    // Descarregar inimigos
    for (int i = 0; i < MAX_ENEMIES; i++) {
        UnloadEnemy(&enemies[i]);
    }
    UnloadTexture(magicTex);
    UnloadTexture(background1);
    CloseWindow();
}
