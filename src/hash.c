#include "../include/hash.h"

TRANSPOSITION *CreateHashTable()
{
	// Cria a tabela de transposição com TABLE_SIZE entradas;

	TRANSPOSITION *hash_table = (TRANSPOSITION *)malloc(TABLE_SIZE*sizeof(TRANSPOSITION));
	
	// Limpa a tabela de transposição;

	ClearHashTable(hash_table);
	
	return(hash_table);
}

void InitHashTable(ulong zobrist_table[128])
{
	// Inicializa os valores para o zobrist hashing;

	// Gera a semente para gerar valores
	// aleatórios baseando-se no horário;

	srand(time(NULL));

	// Percorre todas as entradas da zobrist_table
	// e associa a cada uma delas um número aleatório;

	for (int i = 0; i < 128; i++)
	{
		zobrist_table[i]=rand();
	}

	return;
}

void ClearHashTable(TRANSPOSITION *hash_table)
{
	// A cada entrada da hash table, define-se
	// a profundidade daquele cenário como -1
	// (Condição de entrada vazia);
	
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		hash_table[i].depth = -1;
	}
	return;
}

ulong CreateHash(BOARD board, int depth, ulong zobrist_table[128])
{
	// De início, a hash é nula;

	ulong hash=0;
	
	// Cria-se uma bitboard contendo
	// uma entrada não nula para
	// percorrer todo o tabuleiro,
	// para analisar casa a casa
	// das bitboards de board;

	ulong cell_board;
	
	// Percorre entre os jogadores
	// k = BLACK = 0 e k = WHITE = 1;
	for (int k = 0; k < 2; k++)
	{
		// A bitboard contendo uma entrada
		// começa da seguinte maneira:
		cell_board=0x8000000000000000;
		/*
			1 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
		*/
		// Percorre toda a zobrist table
		// e calcula o zobrist hash para o tabuleiro
		// dado, fazendo operações bitwise XOR;

		for (int i = 0; i < 64; i++)
		{
			if ((cell_board & board.playerBoards[k]) != 0)
			{
				hash ^= zobrist_table[i+64*k];
			}
			
			// Desloca o tabuleiro cell_board uma casa;

			cell_board = cell_board >> 1;
		}
	}
	
	// Para diferenciar tabuleiros que foram
	// analisados em diferentes profundidades,
	// soma-se a depth ao hash e faz-se com que ela
	// fique contida dentro de TABLE_SIZE;

	hash=(hash + depth) % TABLE_SIZE;

	return hash;
}

void HashInsert(BOARD board, int depth, int eval, ulong zobrist_table[128], TRANSPOSITION *hash_table)
{
	// Obtém a respectiva hash do tabuleiro;

	ulong position = CreateHash(board, depth, zobrist_table);
	
	// Se a casa com a respectiva hash na table de transposição
	// estiver vazia, então preenche ela;
	if (hash_table[position].depth == -1)
	{
		hash_table[position].board = board;
		hash_table[position].depth = depth;
		hash_table[position].eval = eval;
		return;
	}
		
	// Se não, executa um algoritmo chamado
	// quadratic hash, que percorre a tabela
	// de transposição procurando uma célula
	// vazia, a uma distância quadrada do início
	// (1 - 4 - 9 - ...)
	
	// Salva a posição inicial;

	int init_pos=position;
	
	// Define a variável de deslocamento;

	int offset=1;
	
	// Desloca a posição;

	position+=offset;

	// Enquanto não encontrar uma casa vazia, dado um número
	// de tentativas igual a sqrt(TABLE_SIZE), incrementa a distância;

	while (hash_table[position%TABLE_SIZE].depth != -1 && (position-init_pos) < (int)sqrt(TABLE_SIZE))
	{	
		// Aumenta a distância e desloca a posição;
		offset+=2;
		position+=offset;
	}
	
	// Se achou uma casa vazia, preenche-a. Se não, houve colisão
	// ou a tabela está cheia;

	if (hash_table[position%TABLE_SIZE].depth == -1)
	{
		hash_table[(position)%TABLE_SIZE].board = board;
		hash_table[(position)%TABLE_SIZE].depth = depth;
		hash_table[(position)%TABLE_SIZE].eval = eval;
	}
		
	return;
}

int HashSearch(BOARD board, int depth, ulong zobrist_table[128], TRANSPOSITION *hash_table)
{
	// Obtém a respectiva hash do tabuleiro atual;

	ulong position = CreateHash(board, depth, zobrist_table);
	
	// A variável found diz se encontrou-se uma transposição ou não;

	bool found=1;
	
	// Verifica se as informações do tabuleiro na hash table coincidem
	// se não, não encontrou-se a transposição desejada;

	if (hash_table[position].board.playerBoards[0] != board.playerBoards[0])
	{
		found=0;
	}
	if (hash_table[position].board.playerBoards[1] != board.playerBoards[1])
	{
		found=0;
	}
	if (hash_table[position].depth < depth)
	{
		found=0;
	}
	

	// Novamente, utiliza-se quadratic hashing para
	// percorrer a tabela aos quadrados e procurar
	// por uma transposição;

	// Salva a posição inicial;

	int init_pos=position;
	
	// Define a variável de deslocamento;
	int offset=1;
	
	// Desloca a posição;
	position+=offset;

	while (!found && (position-init_pos) < (int)sqrt(TABLE_SIZE))
	{
		// Aumenta a distância e desloca a posição, enquanto não se encontra
		// uma transposição;
		found=1;
		offset+=2;
		if (hash_table[(position)%TABLE_SIZE].board.playerBoards[0] != board.playerBoards[0])
		{
			found=0;
		}
		if (hash_table[(position)%TABLE_SIZE].board.playerBoards[1] != board.playerBoards[1])
		{
			found=0;
		}
		if (hash_table[(position)%TABLE_SIZE].depth != depth)
		{
			found=0;
		}

		position+=offset;
	}

	if (found)
	{
		// Se uma transposição fora encontrada, retorna seu valor
		// se de quem era a vez na transposição for igual a de quem
		// é a vez agora, se não retorna seu valor oposto;

		if (hash_table[position%TABLE_SIZE].board.turn == board.turn)
		{
			return(hash_table[position%TABLE_SIZE].eval);
		} else
		{
			return(-hash_table[position%TABLE_SIZE].eval);
		}
	}
	
	// Caso não se tenha encontrado uma transposição,
	// retorna uma condição de parada;

	return(-INT_MAX);
}
