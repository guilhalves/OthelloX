#pragma once
#include "core.h"
#include "hash.h"
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/*
typedef struct MPC_PARAM
{
	int depth;
	float threshold;
	float slope, offset, dev; 
} param[65][13][2];
*/
typedef struct _THREAD_ATTR
{
	pthread_mutex_t mutex;
	BOARD board;
	int depth;
	bool passed;
	double alpha;
	double beta;
	clock_t initial_time;
	ulong *zobrist_table;
	TRANSPOSITION *hash_table;
	double *pos_evals;
	int index;
	int eval_index;
} THREAD_ATTR;

// A função EvalFunction analisa
// o quão vantajoso o tabuleiro é
// para os jogadores, atribuindo ao
// tabuleiro um número real.
// Obs: Se o número real for negativo
// significa que o jogador da vez está em
// desvantagem, caso contrário ele está em
// vantagem. No caso em que o número real
// seja 0, trata-se de um empate.

double EvalFunction(BOARD board, int eval_index);

// Dado dois números a e b, retorna o maior
// deles;

double Max(double a, double b);

// A função SortMoves ordena os lances possíveis
// em uma ordem que representa a provável qualidade
// de cada lance;

void SortMoves(BOARD *boards, int num_moves, int depth, ulong zobrist_table[128], TRANSPOSITION *hash_table);

// A função NegaMax é um algoritmo de busca pelo melhor
// lance da vez, considerando que o oponente jogue os melhores
// lances;

double NegaMax(BOARD board, int depth, bool passed, double alpha, double beta, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table, int eval_index);

// A função SearchBest procura pelo melhor lance dado
// um tempo limite;

ulong SearchBest(BOARD board, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table, int eval_index);

void PlayRandom(BOARD *board);

void GreedyPlayer(BOARD *board);

void *InitSearchThread();
