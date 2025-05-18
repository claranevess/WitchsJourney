#include "raylib.h"
#include "cora.h"
#include "enemy.h"
#include "tools.h"
#include "projetil.h"
#include "wave.h"
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>              

extern const int screenWidth;
extern const int screenHeight;

static Texture2D background1, magicTex;
static Texture2D aguaIcon, terraIcon, ventoIcon, fogoIcon;

static void DrawAttackInventory(Node* tipoAtual, float w, float h);

int obterDadosPowerUp(int *x, int *y, int *numero) {
    FILE *pipe;
    char buffer[128];
    const char *comando_python = "python C:\\Users\\felip\\projetos\\WitchsJourney\\src\\gemini.py";

    pipe = _popen(comando_python, "r");
    if (pipe == NULL) {
        perror("Erro ao abrir o pipe");
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        if (sscanf(buffer, "%d %d %d", x, y, numero) == 3) {
            _pclose(pipe);
            return 0;
        } else {
            fprintf(stderr, "Erro: Formato de saída do Python inválido: '%s'\n", buffer);
        }
    } else {
        fprintf(stderr, "Erro ao ler a saída do script Python.\n");
    }

    _pclose(pipe);
    return -1;
}


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
    int positionPX, positionPY, powerTipo;

    obterDadosPowerUp(&positionPX, &positionPY, &powerTipo);
    printf("%d %d %d", positionPX, positionPY, powerTipo);

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

            if (projectileW.active && enemies[i].active &&
                CheckCollisionCircleRec(projectileW.position,
                    projectileW.frameRec.width * projectileW.scale * 0.5f,
                    enemies[i].hitbox)) {

                // Verifica se o tipo do projétil é válido
                if (projectileW.tipo == NULL) {
                    printf("Erro: projectileW.tipo é NULL!\n");
                    return;
                }

                int tipoAtaque = projectileW.tipo->valor;
                int tipoDefesa = enemies[i].tipo;

                printf("Debug: tipoAtaque = %d, tipoDefesa = %d\n", tipoAtaque, tipoDefesa);

                if (tipoAtaque < 1 || tipoAtaque > 4 || tipoDefesa < 1 || tipoDefesa > 4) {
                    printf("Erro: Tipo de ataque (%d) ou defesa (%d) inválido!\n", tipoAtaque, tipoDefesa);
                    return;
                }

                float danoBase = 1.0f;
                float multiplicador = GetDamageMultiplier(tipoAtaque, enemies[i].tipo);
                printf("DEBUG (ANTES): multiplicador = %.2f\n", multiplicador);
                float danoFinal = danoBase * multiplicador;

                printf("Debug: Multiplicador calculado = %.2f\n", multiplicador);

                enemies[i].health -= danoFinal;
                projectileW.active = false;

                if (enemies[i].health <= 0) {
                    enemies[i].active = false;
                }
            }

			// decrementação de inimigos vivos
            if (wasAlive && !enemies[i].active) enemiesAlive--;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cora.isAlive)
            ShootProjectile(&projectileW, &cora);
        UpdateProjectile(&projectileW);

        if (enemiesAlive <= 0) {
            wavePtr = wavePtr->next;
            waveNum += 1;
            spawnWave(enemies, MAX_ENEMIES, wavePtr);
            enemiesAlive = wavePtr->normal + wavePtr->boss;
			framesMsg = 180;
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
