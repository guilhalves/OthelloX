#include "intelligence.h"

#define MAX_TIME 1 

const int weights_matrix[64] =
{
	30, -12, 0, -1, -1, 0, -12, 30,
	-12, -15, -3, -3, -3, -3, -15, -12,
	0, -3, 0, -1, -1, 0, -3, 0,
	-1, -3, -1, -1, -1, -1, -3, -1,
	-1, -3, -1, -1, -1, -1, -3, -1,
	0, -3, 0, -1, -1, 0, -3, 0,
	-12, -15, -3, -3, -3, -3, -15, -12,
	30, -12, 0, -1, -1, 0, -12, 30
};

double EvalFunction(BOARD board) {

	double score;
	
	int player_eval=0;
	int player_disks;
	int enemy_eval=0;
	int enemy_disks;
	
	for (int i = 0; i < 64; i++)
	{
		if ((CoordToBit(i/8, i%8) & board.playerBoards[board.turn]) != 0)
		{
			player_eval+=weights_matrix[i];	
		}
		if ((CoordToBit(i/8, i%8) & board.playerBoards[1-board.turn]) != 0)
		{
			enemy_eval+=weights_matrix[i];	
		}
	}
	
	score=player_eval-enemy_eval;
	
	player_disks=DiskCount(board.playerBoards[board.turn], board.turn);

	enemy_disks=DiskCount(board.playerBoards[1-board.turn], 1-board.turn);
	
	int dif = player_disks - enemy_disks;
	int sum = player_disks + enemy_disks;
	
	if (sum >= 40)
	{
		return(dif);
	}
	
	return(score);
	//double weight = -64*atan(sum-40)+128;
	//return((dif*weight+score*(256-weight))/1000);
	
}	


double Max(double a, double b) {
	if (a >= b) {
		return(a);
	}
	return(b);
}

double NegaMax(BOARD board, int depth, bool passed, double alpha, double beta, clock_t initial_time) {

	if (depth == 0) {
		return(EvalFunction(board));
	}	
	
	if ((float)(clock()-initial_time)/CLOCKS_PER_SEC >= MAX_TIME)
	{
		return(-INT_MAX);
	}

	double max_eval=-INT_MAX;

	double eval;

	ulong move_board;
	BOARD temp_board;

	ulong legal_board=GetLegalMoves(board);

	if (legal_board == 0)
	{
		if (passed)
		{
			return(EvalFunction(board));
		}
		board.turn=1-board.turn;
		return(-NegaMax(board, depth, true, -alpha, -beta, initial_time));
	}
	

	// SORT FUNCTION

	for (int i = 0; i < 64; i++)
	{
		move_board=CoordToBit(i/8, i%8);
		if ((move_board & legal_board) != 0)
		{
			temp_board=board;
			Play(&temp_board, move_board);
			eval=-NegaMax(temp_board, depth-1, false, -alpha, -beta, initial_time);
			if (eval >= beta)
			{
				return(eval);
			}
			alpha=Max(alpha, eval);
			max_eval=Max(max_eval, eval);
		}
	}

	return(max_eval);
}



ulong SearchBest(BOARD board, clock_t initial_time)
{
	int depth=0;

	double eval;

	double alpha=-INT_MAX;
	double beta=INT_MAX;
	double max_eval;

	BOARD temp_board;

	int x, y;

	ulong legal_board=GetLegalMoves(board);
	ulong best_move;
	ulong temp_best_move;
	ulong move_board;

	int disks_remaining = 64-(DiskCount(board.playerBoards[0], 0)+DiskCount(board.playerBoards[1], 1));

	while ((float)(clock()-initial_time)/CLOCKS_PER_SEC < MAX_TIME && depth <= disks_remaining)
	{
		best_move=temp_best_move;
		alpha=-INT_MAX;
		for (int i = 0; i < 64; i++)
		{
			move_board=CoordToBit(i/8, i%8);
			if ((move_board & legal_board) != 0)
			{
				temp_board=board;
				
				Play(&temp_board, move_board);
				
				eval=-NegaMax(temp_board, depth, false, -alpha, -beta, initial_time);
				
				if (fabs(fabs(eval)-INT_MAX) > 1.0e-3)
				{
					if (alpha < eval)
					{
						x=i%8;
						y=i/8;
						alpha=eval;
						max_eval=eval;
						temp_best_move=move_board;
					}	
				} else
				{
					break;
				}

			}
		}
		depth++;
	}
	
	printf("Best move found at (%d,%d)\nEval: %lf, Depth: %d\n", x, y, max_eval, depth);
	printf("It took %f seconds to find it\n\n", (float)(clock()-initial_time)/CLOCKS_PER_SEC);
	return(best_move);
}
