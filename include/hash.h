#pragma once
#include "core.h"
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

// Define-se uma struct TRANSPOSITION
// que guardará informações acerca de cenários
// na tabela de transposição;

typedef struct _TRANSPOSITION
{
	BOARD board;
	int depth;
	int eval;
} TRANSPOSITION;

// Define-se um tamanho para a hash table, digamos
// 256 MB, calcula-se o número de entradas da hash
// table da seguinte maneira: TABLE_SIZE = 10*1024*1024/sizeof(BOARD);
// Ou seja, pega-se o tamanho da table em bytes e divide-se pela quantidade
// de bytes de cada transposição;

#define TABLE_SIZE (int)(256*1024*1024/sizeof(TRANSPOSITION))

// A função CreateHashTable cria uma tabela de transposição;

TRANSPOSITION *CreateHashTable();

// A função InitHashTable inicializa os valores de uma tabela
// de zobrist para implementar o zobrist hashing;

void InitHashTable(ulong zobrist_table[128]);

// A função ClearHashTable limpa a tabela de transposição;

void ClearHashTable(TRANSPOSITION *hash_table);

// A função CreateHash gera a respectiva hash (chave) de um dado tabuleiro;

ulong CreateHash(BOARD board, int depth, ulong zobrist_table[128]);

// A função HashInsert insere os dados de um cenário na tabela de transposição;

void HashInsert(BOARD board, int depth, int eval, ulong zobrist_table[128], TRANSPOSITION *hash_table);

// A função HashSearch busca pelos dados de um cenário na tabela de transposição;

int HashSearch(BOARD board, int depth, ulong zobrist_table[128], TRANSPOSITION *hash_table);

