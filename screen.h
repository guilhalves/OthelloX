#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "core.h"
#include "intelligence.h"

void DrawUpLine(SDL_Renderer *renderer, SDL_Rect h_rect, int center_x, int center_y, int size);

void DrawMidLine(SDL_Renderer *renderer, SDL_Rect h_rect, int center_x, int center_y, int size);

void DrawDownLine(SDL_Renderer *renderer, SDL_Rect h_rect, int center_x, int center_y, int size);

void DrawLeftUpLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size);

void DrawLeftDownLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size);

void DrawRightUpLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size);

void DrawRightDownLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size);

void DrawDigit(SDL_Renderer *renderer, int center_x, int center_y, int size, int digit);

void DrawNumber(SDL_Renderer *renderer, int center_x, int center_y, int size, int number);

void DrawCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius);

void DrawHollowCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius);

void DrawCross(SDL_Renderer *renderer, int center_x, int center_y, int length);

void DrawPlus(SDL_Renderer *renderer, int center_x, int center_y, int length);

void ShowMenu(SDL_Window *window, SDL_Renderer *renderer, NODE *node);

void CreateWindow(NODE *node);
