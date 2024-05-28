#pragma once
#include <stdbool.h>

#define DEPTH 1
#define SIZE 8 

#define CIRCLE 0
#define CROSS 1
#define EMPTY -1

typedef struct _COORD {
	int row;
	int column;
} COORD;

typedef struct _NODE {
	char board[SIZE][SIZE];
	int num_disks[2];
	bool player;
} NODE;

void ResetBoard(NODE *node);

bool InBoard(COORD cell);

int Simulate(NODE *node, COORD cell, bool play);
