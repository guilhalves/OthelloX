#include "../include/intelligence.h"

// Define-se o tempo máximo para a máquina pensar

#define MAX_TIME 0.5

// Define-se um tabuleiro de pesos;

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

double EvalFunction(BOARD board)
{
	// score: Pontuação de pesos;

	int score;
	
	// player_eval e enemy_eval são pontuações
	// respetivas ao pesos das peças dos jogadores;
	//
	// player_disks e enemy_disks é a quantidade de
	// discos dos jogadores;
	
	int player_eval=0;
	int player_disks;
	int enemy_eval=0;
	int enemy_disks;
	
	// Percorre o tabuleiro e soma os pesos das peças
	// de cada jogador em suas respectivas pontuações;

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
	
	// Obtém quantos discos cada jogador possui

	player_disks=DiskCount(board.playerBoards[board.turn]);

	enemy_disks=DiskCount(board.playerBoards[1-board.turn]);
	
	// Calcula o score fazendo a diferença de pesos;

	score=player_eval-enemy_eval;
	
	// Define-se diferença e soma de peças e as calcula;

	int dif = player_disks - enemy_disks;
	int sum = player_disks + enemy_disks;
	
	// Retorna uma pontuação final onde se atribui um peso para o score
	// e um peso para a diferença de peças a depender do estágio do jogo
	// (sum);

	// Essa função foi aprimorada com um tempo, inicialmente ela dividia
	// o jogo em 2 estágios, para diferenciar o fim de jogo e meio de jogo,
	// depois ela passou a diferenciar em 4 estágios, e agora ela está
	// diferenciando continuamente os estágios; A expressão atual
	// foi obtida realizando testes entre diferentes funções de avaliação,
	// e também tentando suavizar o gráfico obtido pelas funções de 2 e 4 estágios;

	return((1600-400*(sum/16.0)*atan(sum/32.0))*score+(400*(sum/16.0)*atan(sum/32.0))*dif);
}	

double Max(double a, double b) {
	// Retorna o máximo entre a e b;
	if (a >= b) {
		return(a);
	}
	return(b);
}

double NegaMax(BOARD board, int depth, bool passed, double alpha, double beta, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table)
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
		return(EvalFunction(board));
	}	

	// Se o tempo esgotou, retorna uma condição
	// de parada (A avaliação é -INT_MAX);

	if ((float)(clock()-initial_time)/CLOCKS_PER_SEC >= MAX_TIME)
	{
		return(-INT_MAX);
	}
	
	// eval é uma pontuação associada a um tabuleiro;

	double eval;
	
	// Procura por uma transposição na hash_table;
	
	eval=HashSearch(board, depth, zobrist_table, hash_table);


	// Se o valor da transposição é uma condição de parada,
	// não se encontrou uma transposição e portanto não se 
	// deve retornar seu valor;
	// Logo, retorna o valor somente se for diferente
	// da condição de parada;
	if (eval != -INT_MAX)
	{
		return(eval);
	}
	
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
			eval=EvalFunction(board);
			
			// Insere o cenário atual na hash_table;
			
			HashInsert(board, depth, eval, zobrist_table, hash_table);
			
			return(eval);
		}

		// Caso contrário, passa a vez e continua o algoritmo;

		board.turn=1-board.turn;
		
		eval=-NegaMax(board, depth, true, -beta, -alpha, initial_time, zobrist_table, hash_table);
			
		// Insere o cenário atual na hash_table;
		
		HashInsert(board, depth, eval, zobrist_table, hash_table);
		
		return(eval);
	}
	
	// Obtém o número de movimentos e um vetor que
	// contém bitboards as dos lances possíveis

	int num_moves=0;
	
	ulong *positions = GetCoords(legal_board, &num_moves);
	
	// Cria um vetor que conterá os tabuleiros
	// resultantes da posição atual;

	BOARD boards[num_moves];
	
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

		eval=-NegaMax(boards[i], depth-1, false, -beta, -alpha, initial_time, zobrist_table, hash_table);
		
		// O limite inferior do intervalo [alpha, beta]
		// passa a ser o máximo entre a eval e o antigo alpha;
		// (Se eval > alpha, restringimos o intervalo para [eval, beta];

		alpha=Max(alpha, eval);
		
		// Se o alpha >= beta, o intervalo é vazio,
		// e portanto não há lance satisfatório
		// para a máquina;

		if (alpha >= beta)
		{
			
			// Insere o cenário atual na hash_table;
			
			HashInsert(board, depth, beta, zobrist_table, hash_table);
			
			// Assim sendo, libera o vetor de lances
			// possíveis e realiza o pruning retornando
			// o limite superior do intervalo;

			free(positions);
			return(beta);
		}

	}
	
	// Insere o cenário atual na hash_table;

	HashInsert(board, depth, alpha, zobrist_table, hash_table);
	
	
	// Se passou pelo algoritmo, um lance ótimo foi encontrado,
	// e esse é a avaliação máxima, que é alpha, pois o intervalo
	// [alpha, beta] foi restringido. Assim sendo, libera
	// o vetor de lances possíveis e retorna alpha;

	free(positions);
	return(alpha);

}

ulong SearchBest(BOARD board, clock_t initial_time, ulong zobrist_table[128], TRANSPOSITION *hash_table)
{	
	// Define-se a profundidade inicial para
	// o algoritmo NegaMax;

	int depth=0;
	
	// Define-se variáveis de
	// avaliação temp e prev
	// para comparar dois lances
	// que foram testados;
	
	double temp_eval;
	double prev_eval;
	
	// Define-se o limite inferior
	// do intervalo [alpha, beta]
	// para que seja possível realizar alpha
	// beta pruning;
	
	double alpha=-INT_MAX;
	
	// out_of_time é uma variável que diz se o tempo
	// da máquina acabou ou não;

	bool out_of_time=0;
	
	// Define-se dois tabuleiros contendo posições de lances:
	// best_move e temp_best_move, esses serão comparados depois;
	
	ulong best_move;
	ulong temp_best_move;
	
	// Obtém a quantidade e os lances possíveis;
	
	int num_moves=0;
	ulong *positions = GetCoords(GetLegalMoves(board), &num_moves);

	// Cria-se um vetor de tabuleiros resultantes da posição atual;

	BOARD boards[num_moves];
	
	// Cria-se um vetor com as avaliações de cada tabuleiro;
	
	double pos_evals[num_moves];

	// Dado as posições de lance, guarda o cenário
	// resultante daquele lance na sua respectiva
	// posição do vetor boards;
	
	for (int i = 0; i < num_moves; i++)
	{
		boards[i]=board;
		Play(&boards[i], positions[i]);
	}

	// Calcula a quantidade de discos atual no tabuleiro (sum)
	// e a quantidade de discos que estão vazios no tabuleiro (disks_remaining);

	int sum=(DiskCount(board.playerBoards[0])+DiskCount(board.playerBoards[1]));
	int disks_remaining = 64-sum;
	
	// Define-se o mutex da thread (Para que elas terminem de
	// executar juntas);
	
	pthread_mutex_t mutex;
	
	// Inicia o mutex;
	
	pthread_mutex_init(&mutex, NULL);

	// Define-se as threads de pesquisa, que serão
	// uma para cada lance possível;

	pthread_t search_threads[num_moves];

	// Define-se os atributos que uma thread recebe;

	THREAD_ATTR resources;
	
	// Insere os atributos;

	resources.mutex=mutex;
	resources.passed=false;
	resources.alpha=INT_MAX;
	resources.initial_time=initial_time;
	resources.zobrist_table=zobrist_table;
	resources.hash_table=hash_table;
	resources.pos_evals=pos_evals;

	// Enquanto houver tempo sobrando e a profundidade a ser pesquisada
	// for menor ou igual a quantidade de casas vazias no tabuleiro,
	// realiza Iterative Deepening (aumenta a profundidade de pesquisa
	// do NegaMax aos poucos);

	while ((float)(clock()-initial_time)/CLOCKS_PER_SEC < MAX_TIME && depth <= disks_remaining)
	{	
		// Por enquanto, o melhor lance é o melhor lance temporário;

		best_move=temp_best_move;
		
		// Passa os novos atributos para a thread;
		resources.depth=depth;
		resources.beta=-INT_MAX;

		// Define-se o limite inferior do intervalo [alpha, beta]
		alpha=-INT_MAX;

		// Inicia as num_moves threads que realizarão o algoritmo
		// NegaMax em cada um dos tabuleiros resultantes;

		for (int i = 0; i < num_moves; i++)	
		{
			// Passa os novos atributos para a thread;

			resources.board=boards[i];
			resources.index=i;
			
			while (pthread_create(&search_threads[i], NULL, &InitSearchThread, &resources) != 0){}
			while (pthread_join(search_threads[i], NULL) != 0){}
			
		}
		
		// Analisa as avaliações de cada lance
		// e procura por uma condição de parada
		// (TEMPO ESGOTADO)

		for (int i = 0; i < num_moves; i++)	
		{
			if (fabs(resources.pos_evals[i]) == INT_MAX)
			{
				out_of_time=1;
			}
		}

		// Se o tempo não acabou, compara os lances analisados;
		
		if (!out_of_time)
		{
			
			for (int i = 0; i < num_moves; i++)
			{
				// Se a avaliação é melhor que a anterior;
				// o melhor lance no momento é o em questão;

				if (alpha < resources.pos_evals[i])
				{
					alpha=resources.pos_evals[i];
					temp_best_move=positions[i];
				} else if (alpha == resources.pos_evals[i])
				{
					// Se a avaliação é igual a anterior,
					// verifica no tabuleiro de pesos
					// qual lance é melhor;

					// Obtém os pesos dos lances sendo comparados;

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

					// Se o peso do lance anterior é menor que o atual,
					// então o atual é melhor;

					if (prev_eval < temp_eval)
					{
						temp_best_move=positions[i];
					}
				}
			}
			
			// Define-se o limite superior
			// do intervalo [alpha, beta];

			resources.beta=alpha;
		}

		// Aumenta a profundidade;

		depth++;
	}
	
	// Destrói o mutex após todas as threads serem liberadas;
		
	pthread_mutex_destroy(&mutex);
	
	// Libera o vetor positions;
	
	free(positions);

	// Retorna o melhor lance encontrado;

	printf("DEPTH: %d\n", depth);
	return(best_move);
}

void *InitSearchThread(void *arg)
{

	// Recebe os atributos da thread;

	THREAD_ATTR *r = (THREAD_ATTR *)arg;
	
	// Faz um lock no mutex, para que ele não prossiga
	// enquanto não tiver terminado de executar o algoritmo abaixo;

	pthread_mutex_lock(&((*r).mutex));
	
	// Executa uma instância do algoritmo NegaMax;

	(*r).pos_evals[(*r).index]=-NegaMax((*r).board, (*r).depth, (*r).passed, -(*r).alpha, -(*r).beta, (*r).initial_time, (*r).zobrist_table, (*r).hash_table);
	
	// Libera o lock, podendo prosseguir o código de SearchBest
	// se nenhuma outra thread estiver em lock;

	pthread_mutex_unlock(&((*r).mutex));
	
	return(r);
}

