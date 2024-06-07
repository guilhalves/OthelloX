#pragma once
#include "core.h"
#include <math.h>
#include <limits.h>
#include <time.h>

// A função EvalFunction analisa
// o quão vantajoso o tabuleiro é
// para os jogadores, atribuindo ao
// tabuleiro um número real.
// Obs: Se o número real for negativo
// significa que o jogador da vez está em
// desvantagem, caso contrário ele está em
// vantagem. No caso em que o número real
// seja 0, trata-se de um empate.

double EvalFunction(BOARD board);

// Dado dois números a e b, retorna o maior
// deles;

double Max(double a, double b);

// A função NegaMax é um algoritmo de busca pelo melhor
// lance da vez, considerando que o oponente jogue os melhores
// lances;

double NegaMax(BOARD board, int depth, bool passed, double alpha, double beta, clock_t initial_time);

// A função SearchBest procura pelo melhor lance dado
// um tempo limite;

ulong SearchBest(BOARD board, clock_t initial_time);
