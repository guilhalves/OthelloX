#include "intelligence.h"
#include "screen.h"

const int weight_matrix[8][8] =
{
	{30, -15, 0, -1, -1, 0, -15, 30},
	{-15, -12, -3, -3, -3, -3, -12, -15},
	{0, -3, 0, -1, -1, 0, -3, 0},
	{-1, -3, -1, -1, -1, -1, -3, -1},
	{-1, -3, -1, -1, -1, -1, -3, -1},
	{0, -3, 0, -1, -1, 0, -3, 0},
	{-12, -15, -3, -3, -3, -3, -15, -12},
	{30, -12, 0, -1, -1, 0, -12, 30}
};

COORD *GetChilds(NODE node, int *num_moves) {

	COORD *childs;

	COORD temp_childs[SIZE*SIZE];

	COORD cell;

	int changes;

	*num_moves=0;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			cell.row=i;
			cell.column=j;

			changes=Simulate(&node, cell, 0);

			if (changes > 0) {
				temp_childs[*num_moves]=cell;
				*num_moves=*num_moves+1;
			}
		}
	}
	childs=(COORD *)malloc(*num_moves*sizeof(COORD));

	for (int k = 0; k < *num_moves; k++) {
		childs[k]=temp_childs[k];
	}

	return(childs);
}

double EvalFunction(NODE node) {

	int score;

	int player_eval=0;
	int enemy_eval=0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (node.board[i][j] == node.player)
			{
				player_eval+=weight_matrix[i][j];
			}
			if (node.board[i][j] == (node.player+1)%2)
			{
				enemy_eval+=weight_matrix[i][j];
			}
		}
	}

	if (node.num_disks[node.player] == 0)
	{
		return(-INT_MAX);
	}

	if (node.num_disks[(node.player+1)%2] == 0)
	{
		return(INT_MAX);	
	}

	score=player_eval/node.num_disks[node.player]-enemy_eval/node.num_disks[(node.player+1)%2];

	return(score);
}



int Max(int a, int b) {
	if (a >= b) {
		return(a);
	}
	return(b);
}

double NegaMax(NODE node, int depth, bool passed, double alpha, double beta) {

	if (depth == 0) {
		return(EvalFunction(node));
	}	

	double max_eval=-INT_MAX;

	int num_moves;

	double eval;

	NODE temp_node;

	COORD *childs;

	childs=GetChilds(node, &num_moves);

	if (num_moves == 0) {
		if (passed)
		{
			free(childs);
			return(EvalFunction(node));
		}
		free(childs);
		node.player=(node.player+1)%2;
		return(-NegaMax(node, depth, true, -alpha, -beta));
	}

	for (int k = 0; k < num_moves; k++) {

		temp_node=node;

		Simulate(&temp_node, childs[k], 1);

		eval=-NegaMax(temp_node, depth - 1, false, -alpha, -beta);

		if (eval >= beta)
		{
			free(childs);
			return(eval);
		}

		alpha=Max(alpha, eval);

		max_eval=Max(max_eval, eval);
	}


	free(childs);
	return(max_eval);
}

COORD SearchBest(NODE node, int depth)
{
	int num_moves;

	double eval;

	double alpha=-INT_MAX;
	double beta=INT_MAX;

	NODE temp_node;

	COORD best_move;

	COORD *childs;

	childs=GetChilds(node, &num_moves);

	for (int k = 0; k < num_moves; k++)
	{
		temp_node=node;

		Simulate(&temp_node, childs[k], 1);

		eval=-NegaMax(temp_node, depth, false, -alpha, -beta);
		if (alpha < eval) {
			alpha=eval;
			best_move=childs[k];
			printf("Temporary best move: (%d, %d), eval: %lf\n", best_move.row, best_move.column, eval);
		}
	}


	free(childs);
	return(best_move);
}
