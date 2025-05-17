#ifndef DRAW_H
#define DRAW_H

#include <raylib.h>

extern Texture2D backgroundmenu;
extern Texture2D backgroundinstrucoes;
extern Texture2D backgroundraiz;
extern Texture2D background1;
extern Texture2D background2;
extern Texture2D background3;
extern Texture2D background4;
extern Texture2D player;
extern Texture2D enemy;
extern Texture2D enemy2;
extern Texture2D enemy3;
extern Texture2D enemy4;
extern Texture2D enemy5;
extern Texture2D enemy6;
extern Texture2D backgroundleaderboard;
extern Texture2D aguaIcon, terraIcon, ventoIcon, fogoIcon;

void InitAssets(void);
void UnloadAssets(void);
void DrawGame(void);

#endif
