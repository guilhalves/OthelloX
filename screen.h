#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "core.h"
#include "intelligence.h"

// A função DrawCircle desenha uma peça; 

void DrawCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius);

// A função CreateWindow
// inicia a janela do jogo;

void CreateWindow(BOARD *board);
