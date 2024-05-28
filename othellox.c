#include "core.h"
#include "screen.h"
#include "intelligence.h"
#include <stdio.h>

int main() {
	
	int eval=0;

	NODE node;
	COORD cell;
	cell.row=0;
	cell.column=0;
	
	//WeightCalculator(node);

	ResetBoard(&node);

	CreateWindow(&node);

	/*
	while (no_move_rounds != 2) {
		
		if (no_move_rounds > 0) {
			break;
		}

		printf("\n");	
	
		
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == -1) {
					printf(". ");
				} else {
					printf("%d ", board[i][j]);
				}
			}
			printf("\n");
		}
		printf("\n");	
		
		//eval=MiniMax(node, board, DEPTH, -INT_MAX, INT_MAX, &cell);
		//printf("Best move found in (%d, %d): eval %d\n", cell.row, cell.column, eval);
		//scanf("%d", &jog_sem_lance);
		
		//jog_sem_lance=0;

		PrintBoard(node, cell, &no_move_rounds);
		
		eval=MiniMax(node, DEPTH, -INT_MAX, INT_MAX, &cell);
		
		printf("Best move found in (%d, %d): eval %d\n", cell.row, cell.column, eval);
		
		node=Simulate(node, cell, &changes, 1, &no_move_rounds);
		
			
		while (node.player == 0) {
			PrintBoard(node, cell, &no_move_rounds);
			node=ReadKey(node, &cell, &no_move_rounds);		
		}
		
		while (node.player == 1) {
			PrintBoard(node, cell, &no_move_rounds);
			node=ReadKey(node, &cell, &no_move_rounds);		
		}
		
	}
	*/
	return(0);
}
