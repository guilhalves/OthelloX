#pragma once
#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

extern const int weight_matrix[8][8];

COORD *GetChilds(NODE node, int *num_moves);

double EvalFunction(NODE node);

int Max(int a, int b);

double NegaMax(NODE node, int depth, bool passed, double alpha, double beta);

COORD SearchBest(NODE node, int depth);
