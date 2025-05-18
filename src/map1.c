#include "raylib.h"
#include "cora.h"
#include "enemy.h"
#include "tools.h"
#include "projetil.h"
#include "wave.h"                 

extern const int screenWidth;
extern const int screenHeight;

static Texture2D background1, magicTex;
static Texture2D aguaIcon, terraIcon, ventoIcon, fogoIcon;

static void DrawAttackInventory(Node* tipoAtual, float w, float h);

void map1(void)
{
    InitWindow(screenWidth, screenHeight, "Mapa 1");
    SetTargetFPS(60);

    background1 = LoadTexture("resources/assets/background1.png");
    aguaIcon = LoadTexture("resources/assets/icons/agua_icon.png");
    terraIcon = LoadTexture("resources/assets/icons/terra_icon.png");
    ventoIcon = LoadTexture("resources/assets/icons/vento_icon.png");
    fogoIcon = LoadTexture("resources/assets/icons/fogo_icon.png");

    // Projeteis
    Texture2D projectileTextures[4];
    projectileTextures[0] = LoadTexture("resources/assets/proj_agua.png");
    projectileTextures[1] = LoadTexture("resources/assets/proj_terra.png");
    projectileTextures[2] = LoadTexture("resources/assets/proj_vento.png");
    projectileTextures[3] = LoadTexture("resources/assets/proj_fogo.png");


    Cora cora = initCora();
    Projectile projectileW;
    InitProjectile(&projectileW, projectileTextures);

#define MAX_ENEMIES 30
    Enemy enemies[MAX_ENEMIES];

    // inicializa a lista circular
    WaveNode* wavePtr = createWave();              // começa na Horda 1
    spawnWave(enemies, MAX_ENEMIES, wavePtr);      // cria inimigos da Horda 1
    int enemiesAlive = wavePtr->normal + wavePtr->boss;

    int waveNum = 1;      // 1–5
    int framesMsg = 180;    // exibe “HORDA N!” por 3 s
    bool showHit = false;  // tecla H mostra hitboxes

    while (!WindowShouldClose())
    {
        updateCora(&cora);
        if (IsKeyPressed(KEY_H)) showHit = !showHit;

        // update dos inimigos + colisão
        for (int i = 0; i < MAX_ENEMIES; i++) {
            bool wasAlive = enemies[i].active;

            UpdateEnemy(&enemies[i], cora.position, cora.isAlive);

            // colisão cora x inimigo 
            if (enemies[i].active && cora.isAlive &&
                CheckCollisionRecs(cora.hitbox, enemies[i].hitbox))
            {
                cora.health--;
                if (cora.health <= 0) { cora.health = 0; cora.isAlive = false; }
            }

            // colisão projétil x inimigo 
            if (projectileW.active && enemies[i].active &&
                CheckCollisionCircleRec(projectileW.position,
                    projectileW.frameRec.width * projectileW.scale * 0.5f,
                    enemies[i].hitbox))
            {
                enemies[i].health--;
                projectileW.active = false;
                if (enemies[i].health <= 0) enemies[i].active = false;
            }

			// decrementação de inimigos vivos
            if (wasAlive && !enemies[i].active) enemiesAlive--;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cora.isAlive)
            ShootProjectile(&projectileW, &cora);
        UpdateProjectile(&projectileW);

        if (enemiesAlive <= 0) {                           // todos morreram
            wavePtr = wavePtr->next;                       // próxima horda
            waveNum = (waveNum % 5) + 1;                   // 1→2→…→5→1
            spawnWave(enemies, MAX_ENEMIES, wavePtr);      // recria inimigos
            enemiesAlive = wavePtr->normal + wavePtr->boss;// reinicia contagem
			framesMsg = 180;                               // mostra “HORDA N! durante 3s
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background1, 0, 0, WHITE);

        DrawAttackInventory(projectileW.tipo, screenWidth, screenHeight);
        drawCora(&cora);
        DrawProjectile(&projectileW);

        for (int i = 0; i < MAX_ENEMIES; i++) {
            DrawEnemy(&enemies[i]);
            if (enemies[i].active)
                DrawEnemyHealthBar(&enemies[i]);
        }

        DrawDebugHitboxes(&cora, enemies, MAX_ENEMIES, showHit);

        DrawText(TextFormat("Vida: %d", cora.health), 10, 10, 20, WHITE);

        if (!cora.isAlive)
            DrawText("FIM DE JOGO!",
                screenWidth / 2 - 80, screenHeight / 2, 30, WHITE);

        if (framesMsg > 0) {
            DrawText(TextFormat("HORDA %d!", waveNum),
                screenWidth / 2 - MeasureText("HORDA 5!", 40) / 2,
                screenHeight / 2 - 90, 40, YELLOW);
            framesMsg--;
        }

        EndDrawing();
    }

    unloadCora(&cora);
    for (int i = 0; i < MAX_ENEMIES; i++) UnloadEnemy(&enemies[i]);
    UnloadTexture(magicTex);
    UnloadTexture(background1);
    // unload Projeteis
    for (int i = 0; i < 4; i++) {
        UnloadTexture(projectileTextures[i]);
    }
    CloseWindow();
}

static void DrawAttackInventory(Node* tipoAtual, float w, float h)
{
    float startX = w / 2 - 100;
    float y = h - 80;
    float size = 48;
    float spacing = 10;

    Texture2D icons[4] = { aguaIcon, terraIcon, ventoIcon, fogoIcon };

    for (int i = 0; i < 4; i++) {
        float x = startX + i * (size + spacing);
        if (tipoAtual->valor == i + 1)
            DrawRectangleLines(x - 2, y - 2, size + 4, size + 4, YELLOW);
        DrawTextureEx(icons[i], (Vector2) { x, y }, 0.0f, size / icons[i].width, WHITE);
    }
}
