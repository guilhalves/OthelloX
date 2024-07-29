#pragma once
#include "core.h"
#include "hash.h"
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// A struct THREAD_ATTR contém
// os atributos que serão passados
// para uma thread que eventualmente
// for inicializada (Thread algoritmo NegaMax);

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

double EvalFunction(BOARD board);

// Dado dois números a e b, retorna o maior
// deles;

double Max(double a, double b);

// A função NegaMax é um algoritmo de busca pelo melhor
// lance da vez, considerando que o oponente jogue os melhores
// lances;

double NegaMax(BOARD board, int depth, bool passed, double alpha, double beta, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table);

// A função SearchBest procura pelo melhor lance dado
// um tempo limite;

ulong SearchBest(BOARD board, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table);

// A função InitSearchThread inicia uma thread
// que realiza o algoritmo NegaMax;

void *InitSearchThread();
