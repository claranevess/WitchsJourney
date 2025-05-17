#include <raylib.h>
#include "draw.h"

Texture2D backgroundmenu;
Texture2D backgroundinstrucoes;
Texture2D backgroundraiz;
Texture2D background1changed;
Texture2D background1;
Texture2D background2;
Texture2D background3;
Texture2D background4;
Texture2D player;
Texture2D enemy;
Texture2D enemy2;
Texture2D enemy3;
Texture2D enemy4;
Texture2D enemy5;
Texture2D enemy6;
Texture2D backgroundleaderboard;

void InitAssets(void) {
	backgroundmenu = LoadTexture("resources/assets/backgroundmenu.png");
	backgroundinstrucoes = LoadTexture("resources/assets/backgroundinstrucoes.png");
	background1 = LoadTexture("resources/assets/background1.png");
    backgroundraiz = LoadTexture("resources/assets/backgroundraiz.png");
    background2 = LoadTexture("resources/assets/background2.png");
    background3 = LoadTexture("resources/assets/background3.png");
    background4 = LoadTexture("resources/assets/background4.png");

    player = LoadTexture("resources/assets/player.png");    

    enemy = LoadTexture("resources/assets/enemy.png");
    enemy2 = LoadTexture("resources/assets/enemy2.png");
    enemy3 = LoadTexture("resources/assets/enemy3.png");
    enemy4 = LoadTexture("resources/assets/enemy4.png");
    enemy5 = LoadTexture("resources/assets/enemy5.png");
    enemy6 = LoadTexture("resources/assets/enemy6.png");

	backgroundleaderboard = LoadTexture("resources/assets/backgroundleaderboard.png");
}

void DrawGame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

	DrawTexture(backgroundmenu, 0, 0, WHITE);
	DrawTexture(backgroundinstrucoes, 0, 0, WHITE);
    DrawTexture(backgroundraiz, 0, 0, WHITE);
	DrawTexture(background1, 0, 0, WHITE);
	DrawTexture(background2, 0, 0, WHITE);
	DrawTexture(background3, 0, 0, WHITE);
	DrawTexture(background4, 0, 0, WHITE);
	DrawTexture(backgroundleaderboard, 0, 0, WHITE);
    DrawTexture(player, 100, 100, WHITE);
    DrawTexture(enemy, 300, 100, WHITE);
    DrawTexture(enemy2, 350, 100, WHITE);
    DrawTexture(enemy3, 400, 100, WHITE);
    DrawTexture(enemy4, 450, 100, WHITE);
    DrawTexture(enemy5, 500, 100, WHITE);
    DrawTexture(enemy6, 550, 100, WHITE);

    EndDrawing();
}

void UnloadAssets(void) {
	UnloadTexture(backgroundmenu);
	UnloadTexture(backgroundinstrucoes);
    UnloadTexture(backgroundraiz);
	UnloadTexture(background1);
    UnloadTexture(background2);
    UnloadTexture(background3);
    UnloadTexture(background4);

    UnloadTexture(player);

    UnloadTexture(enemy);
    UnloadTexture(enemy2);
    UnloadTexture(enemy3);
    UnloadTexture(enemy4);
    UnloadTexture(enemy5);
    UnloadTexture(enemy6);

	UnloadTexture(backgroundleaderboard);
}
