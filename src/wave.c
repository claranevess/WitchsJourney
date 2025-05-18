#include "wave.h"
#include "raylib.h"   

const int waveEnemyCount[5] = { 10, 10, 15, 15, 20 };  

WaveNode* createWave(void)
{
	// cria hordas de inimigos estáticas (quantidade já atribuída p cada wave)
    static WaveNode w1, w2, w3, w4, w5;

	// faz o loop circular entre as hordas (w1 -> w2 -> w3 -> w4 -> w5) e suas quantidades, e no fim ta retornando pra wave 1

    w1 = (WaveNode){ .normal = 10, .boss = 0, .next = &w2 };
    w2 = (WaveNode){ .normal = 10, .boss = 0, .next = &w3 };
    w3 = (WaveNode){ .normal = 15, .boss = 0, .next = &w4 };
    w4 = (WaveNode){ .normal = 15, .boss = 1, .next = &w5 };
    w5 = (WaveNode){ .normal = 20, .boss = 2, .next = &w1 };

    return &w1;
}

// inicializa os inimigos nas hordas
void spawnWave(Enemy* enemies, int maxSlot, WaveNode* node)
{
    int total = node->normal + node->boss;
    for (int i = 0; i < maxSlot; i++) {
        if (i < total) {
            int tipo;

            // decide o tipo do inimigo: “normal” 1-4, bosses 5
            if (i < node->normal) {
                tipo = GetRandomValue(1, 4);
            }
            else {
                tipo = 5;
            }
            InitEnemy(&enemies[i], tipo);
        }
        else {
            enemies[i].active = false;
        }
    }
}

