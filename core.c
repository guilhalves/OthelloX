#include "core.h"

void ResetBoard(NODE *node) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			(*node).board[i][j]=EMPTY;
		}
	}
	
	(*node).board[(SIZE/2)-1][(SIZE/2)-1]=CROSS;
	(*node).board[SIZE/2][SIZE/2]=CROSS;
	(*node).board[(SIZE/2)-1][(SIZE/2)]=CIRCLE;
	(*node).board[(SIZE/2)][(SIZE/2)-1]=CIRCLE;
	
	(*node).num_disks[CIRCLE]=2;
	(*node).num_disks[CROSS]=2;
	
	(*node).player=CIRCLE;
	
	return;
}

bool InBoard(COORD cell) {
	return(cell.row >= 0 && cell.row < SIZE && cell.column >= 0 && cell.column < SIZE);
}

int Simulate(NODE *node, COORD cell, bool play) {

	int changes=0;

	if ((*node).board[cell.row][cell.column] != EMPTY) {
		return(changes);
	}
	
	int radius=1;
	
	COORD region_cell;

	for (int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			
			radius=0;
			
			do {
				radius++;
				region_cell.row=cell.row+i*radius;
				region_cell.column=cell.column+j*radius;
				
			} while (InBoard(region_cell) && (*node).board[region_cell.row][region_cell.column] == ((*node).player+1)%2);
			
			if (radius > 1 && InBoard(region_cell) && (*node).board[region_cell.row][region_cell.column] == (*node).player) {
				
				changes+=radius-1;
				
				if (play && changes > 0) {
					while (radius >= 0) {
						(*node).board[region_cell.row][region_cell.column]=(*node).player;
						radius--;
						region_cell.row=cell.row+i*radius;
						region_cell.column=cell.column+j*radius;
					}
				}
			}
		}
	}

	if (play && changes > 0) {
		
		changes+=1;
		
		(*node).num_disks[(*node).player]+=changes;
		(*node).num_disks[((*node).player+1)%2]-=changes-1;
		
		(*node).player=((*node).player+1)%2;
	
	}
	
	return(changes);
}
