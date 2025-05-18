#ifndef WAVE_H
#define WAVE_H
#include "enemy.h"

typedef struct WaveNode {
    int normal; // aqui a gente vai setar os inimigos normais (que seria de 1-4) e o boss (que eh do tipo 5)
    int boss;        
    struct WaveNode* next; // ponteiro pra a próxima wave (horda)
} WaveNode;

WaveNode* createWave(void);
void spawnWave(Enemy* enemies, int maxSlot, WaveNode* node);

#endif
