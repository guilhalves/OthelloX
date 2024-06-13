#include "../include/intelligence.h"

#define MAX_TIME 0.5

int moves_realized=0;
int sum_depths=0;

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

double EvalFunction(BOARD board, int eval_index)
{

	int score;
	
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
	
	
	player_disks=DiskCount(board.playerBoards[board.turn]);

	enemy_disks=DiskCount(board.playerBoards[1-board.turn]);

	score=player_eval-enemy_eval;
	
	
	int dif = player_disks - enemy_disks;
	int sum = player_disks + enemy_disks;
	
	if (eval_index == 0)
	{
		return(dif);
	} else if (eval_index == 1)
	{
		/*
		if (sum >= 40)
		{
			return(4*dif);
		}
		*/
		return((1600-400*sum/16)*score+400*(1+sum/16)*dif);

	} else if (eval_index == 2)
	{
		/*
		if (sum >= 40)
		{
			return(4*dif);
		}
		//score=NewEvalFunction(board);
		return(score);
		*/
		return((1600-400*(sum/16.0)*atan(sum/32.0))*score+(400*(sum/16.0)*atan(sum/32.0))*dif);
	}

	
	//double weight = -64*atan(sum-40)+128;
	//return((dif*weight+score*(256-weight))/1000);	
}	

// EXCLUIR (math.h?)
double Max(double a, double b) {
	// Retorna o máximo entre a e b;
	if (a >= b) {
		return(a);
	}
	return(b);
}

void SortMoves(BOARD *boards, int num_moves, int depth, ulong zobrist_table[128], TRANSPOSITION *hash_table)
{
	int pos_evals[num_moves];
	BOARD temp_board;
	//temp_board.playerBoards[1]=0;
	//temp_board.turn=0;

	// Obtém os respectivos evals;
	for (int i = 0; i < num_moves; i++)
	{
		//temp_board.playerBoards[0]=positions[i];
		//pos_evals[i]=EvalFunction(temp_board, eval_index);
			
		pos_evals[i]=HashSearch(boards[i], depth, zobrist_table, hash_table);
	
		//pos_evals[i]=-pos_evals[i];

		if (pos_evals[i] == -INT_MAX)
		{
			pos_evals[i]=EvalFunction(boards[i], 1);
		}
		
	}

	// Bubble sort
	
	for (int i = 0; i < num_moves-1; i++)
	{
		if (pos_evals[i] < pos_evals[i+1])
		{
			temp_board=boards[i];
			boards[i]=boards[i+1];
			boards[i+1]=temp_board;
		}
	}

	return;
}

double NegaMax(BOARD board, int depth, bool passed, double alpha, double beta, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table, int eval_index)
{
	
	// O que pode ser pior do que perder um jogo de Othello?
	// Se não possui peças, retorna -INT_MAX+1, que será
	// analisado na função SearchBest, para descartar o lance;
	
	if (DiskCount(board.playerBoards[board.turn]) == 0)
	{
		return(-INT_MAX+1);
	}


	// Se chegou no fim da árvore,
	// retorna o valor do tabuleiro em questão.

	if (depth == 0) {
		return(EvalFunction(board, eval_index));
	}	

	// Se o tempo esgotou, retorna uma condição
	// de parada (A avaliação é -INT_MAX);

	if ((float)(clock()-initial_time)/CLOCKS_PER_SEC >= MAX_TIME)
	{
		return(-INT_MAX);
	}
	
	// eval é uma pontuação associada a um tabuleiro;

	double eval;
	
	// HASH TABLE

	/*
	if (eval_index == 1)
	{
		eval=HashSearch(board, depth, zobrist_table, hash_table);
	
		if (eval != -INT_MAX)
		{
			return(eval);
		}
	}
	*/
	
	// Obtém a bitboard de lances válidos;
	
	ulong legal_board=GetLegalMoves(board);

	// Se não existem lances válidos, segue;

	if (legal_board == 0)
	{
		// Se no tabuleiro que originou o atual
		// foi passada a vez, significa que o jogo
		// terminou, e nesse caso retorna a avaliação
		// do tabuleiro nesse momento;

		if (passed)
		{
			eval=EvalFunction(board, eval_index);
			
			/*
			if (eval_index == 1)
			{
				HashInsert(board, depth, eval, zobrist_table, hash_table);
			}
			*/
			return(eval);
		}

		// Caso contrário, passa a vez e continua o algoritmo;

		board.turn=1-board.turn;
		
		eval=-NegaMax(board, depth, true, -beta, -alpha, initial_time, zobrist_table, hash_table, eval_index);
		
		/*
		if (eval_index == 1)
		{
			HashInsert(board, depth, eval, zobrist_table, hash_table);
		}*/
		return(eval);
	}
	
	// Obtém o número de movimentos e um vetor que
	// contém bitboards as dos lances possíveis

	int num_moves=0;
	
	ulong *positions = GetCoords(legal_board, &num_moves);
	
	// Cria um vetor que conterá os tabuleiros
	// resultantes da posição atual;

	BOARD boards[num_moves];
	
	// SORT FUNCTION (KILLER MOVES)
	/*		
	if (eval_index == 1)
	{
		SortMoves(boards, num_moves, depth-1, zobrist_table, hash_table);
	}
	*/
	
	// Percorre os tabuleiros resultantes e
	// executa o minimax com alpha-beta pruning;
	
	for (int i = 0; i < num_moves; i++)	
	{
		// Dado as posições de lance, guarda o cenário
		// resultante daquele lance na sua respectiva
		// posição do vetor boards;

		boards[i]=board;
		Play(&boards[i], positions[i]);
		
		// Obtém a avaliação do tabuleiro sendo
		// analisado;

		eval=-NegaMax(boards[i], depth-1, false, -beta, -alpha, initial_time, zobrist_table, hash_table, eval_index);
		
		// O limite inferior do intervalo [alpha, beta]
		// passa a ser o máximo entre a eval e o antigo alpha;
		// (Se eval > alpha, restringimos o intervalo para [eval, beta];

		alpha=Max(alpha, eval);
		
		// Se o alpha >= beta, o intervalo é vazio,
		// e portanto não há lance satisfatório
		// para a máquina;

		if (alpha >= beta)
		{
			/*
			if (eval_index == 1)
			{
				HashInsert(board, depth, beta, zobrist_table, hash_table);
			}*/
			
			
			// Assim sendo, libera o vetor de lances
			// possíveis e realiza o pruning retornando
			// o limite superior do intervalo;

			free(positions);
			return(beta);
		}

	}
	
	/*
	if (eval_index == 1)
	{
		HashInsert(board, depth, alpha, zobrist_table, hash_table);
	}*/
	
	
	// Se passou pelo algoritmo, um lance ótimo foi encontrado,
	// e esse é a avaliação máxima, que é alpha, pois o intervalo
	// [alpha, beta] foi restringido. Assim sendo, libera
	// o vetor de lances possíveis e retorna alpha;

	free(positions);
	return(alpha);

}

ulong SearchBest(BOARD board, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table, int eval_index)
{	
	int depth=0;
	
	double eval;

	double temp_eval;
	double prev_eval;

	double alpha=-INT_MAX;
	double max_eval;
	bool out_of_time=0;
	bool lost=0;

	ulong best_move;
	ulong temp_best_move;
	
	int num_moves=0;
	ulong *positions = GetCoords(GetLegalMoves(board), &num_moves);

	BOARD boards[num_moves];

	double pos_evals[num_moves];

	// Dado as posições de lance, guarda o cenário
	// resultante daquele lance na sua respectiva
	// posição do vetor boards;
	
	for (int i = 0; i < num_moves; i++)
	{
		boards[i]=board;
		Play(&boards[i], positions[i]);
	}

	int sum=(DiskCount(board.playerBoards[0])+DiskCount(board.playerBoards[1]));
	int disks_remaining = 64-sum;

	pthread_mutex_t mutex;
	pthread_t search_threads[num_moves];
	pthread_mutex_init(&mutex, NULL);

	THREAD_ATTR resources;
	
	resources.mutex=mutex;
	resources.passed=false;
	resources.alpha=INT_MAX;
	resources.initial_time=initial_time;
	resources.zobrist_table=zobrist_table;
	resources.hash_table=hash_table;
	resources.pos_evals=pos_evals;
	resources.eval_index=eval_index;

	while ((float)(clock()-initial_time)/CLOCKS_PER_SEC < MAX_TIME && depth <= disks_remaining)
	{
		best_move=temp_best_move;
		resources.depth=depth;
		resources.beta=-INT_MAX;
		alpha=-INT_MAX;

		for (int i = 0; i < num_moves; i++)	
		{
			resources.board=boards[i];
			resources.index=i;
			while (pthread_create(&search_threads[i], NULL, &InitSearchThread, &resources) != 0){}
			while (pthread_join(search_threads[i], NULL) != 0){}
			
		}
		
		pthread_mutex_destroy(&mutex);
		
		lost=0;
		for (int i = 0; i < num_moves; i++)	
		{
			//printf("i: %d, eval: %d\n", i, resources.pos_evals[i]);
			if (abs(resources.pos_evals[i]) == INT_MAX)
			{
				out_of_time=1;
			}
			if (abs(resources.pos_evals[i]) == INT_MAX-1)
			{
				lost=1;
			}
		}
		if (!out_of_time)
		{
			resources.beta=alpha;
			for (int i = 0; i < num_moves; i++)
			{
				if (abs(resources.pos_evals[i]) != INT_MAX)
				{
					//printf("ALPHA: %d, CURR_EVAL: %d\n", alpha, resources.pos_evals[i]);
					if (alpha < resources.pos_evals[i])
					{
						alpha=resources.pos_evals[i];
						max_eval=resources.pos_evals[i];
						temp_best_move=positions[i];
					} else if (alpha == resources.pos_evals[i])
					{
						for (int j = 0; j < 64; j++)
						{
							if ((CoordToBit(j / 8, j % 8) & temp_best_move) != 0)
							{
								prev_eval=weights_matrix[j];
							}
						}
						for (int j = 0; j < 64; j++)
						{
							if ((CoordToBit(j / 8, j % 8) & positions[i]) != 0)
							{
								temp_eval=weights_matrix[j];
							}
						}
						if (prev_eval < temp_eval)
						{
							temp_best_move=positions[i];
							//printf("Substituindo lances supostamente equivalentes\n");
						}
					}
				}
			}
		}
		depth++;
	}
	
	free(positions);

	sum_depths+=depth;
	moves_realized++;
		
	if (eval_index == 1)
	{
		printf("Eval: %lf, Depth: %d\n", max_eval, depth);
	}
		
	//printf("It took %f seconds to find it\n", (float)(clock()-initial_time)/CLOCKS_PER_SEC);
	
	//printf("NODES VISITED: %lu\n", nodes_visited);
	//printf("COLLISIONS: %lu, TRANSPOSITIONS: %lu\n", collisions, transpositions);
	
	//printf("%.2lf%% of the hash table is occupied\n\n", (100*occupied)/(1.0*TABLE_SIZE));
	
	//printf("AVERAGE DEPTH: %.2lf\n", sum_depths/(1.0*moves_realized));
	
	return(best_move);
}

void *InitSearchThread(void *arg)
{
	THREAD_ATTR *resources = (THREAD_ATTR *)arg;
	
	pthread_mutex_lock(&((*resources).mutex));
	
	(*resources).pos_evals[(*resources).index]=-NegaMax((*resources).board, (*resources).depth, (*resources).passed, -(*resources).alpha, -(*resources).beta, (*resources).initial_time, (*resources).zobrist_table, (*resources).hash_table, (*resources).eval_index);
	
	pthread_mutex_unlock(&((*resources).mutex));
	
	return(resources);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayRandom(BOARD *board)
{
	int num_moves=0;
	
	ulong *positions = GetCoords(GetLegalMoves(*board), &num_moves);
	
	srand(time(NULL));
	
	Play(board, positions[rand() % num_moves]);

	free(positions);
	return;
}

void GreedyPlayer(BOARD *board)
{
	int num_moves=0;
	
	ulong *positions = GetCoords(GetLegalMoves(*board), &num_moves);
	
	BOARD boards[num_moves];
	double pos_evals[num_moves];
	int best_index=0;
	
	for (int i = 0; i < num_moves; i++)
	{
		boards[i]=(*board);
		Play(&boards[i], positions[i]);
		pos_evals[i]=DiskCount(boards[i].playerBoards[boards[i].turn])-DiskCount(boards[i].playerBoards[1-boards[i].turn]);
	}
	
	for (int i = 0; i < num_moves; i++)
	{
		if (pos_evals[i] > pos_evals[best_index])
		{
			best_index=i;
		}
	}

	Play(board, positions[best_index]);

	free(positions);
	return;
}

