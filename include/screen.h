#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "core.h"
#include "intelligence.h"

// Define os índices de daltonismo
// para desenhar cores distintas
// na tela;

#define NAO_DALTONICO 0
#define ACROMATOPSIA 1
#define ACROMATOPSIA_AT 2
#define PROTANOPIA 3
#define DEUTERANOPIA 4
#define TRITANOPIA 5
#define PROTANOMALIA 6
#define DEUTERANOMALIA 7
#define TRITANOMALIA 8

// A função DrawCircle desenha uma peça; 

void DrawCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius, bool transparent);

// A função CreateWindow
// inicia a janela do jogo;

void InitGame(BOARD *board, int player, int daltonism, int player_vs_player);
