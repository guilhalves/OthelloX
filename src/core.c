#include "../include/core.h"
#include "../include/intelligence.h"

void ResetBoard(BOARD *board)
{
	// O tabuleiro será representado por uma bitboard;

	(*board).playerBoards[BLACK]=0x810000000;
	
	// Representa o número binário abaixo em hexadecimal
	/*
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 1 0 0 0
		0 0 0 1 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
	*/
	(*board).playerBoards[WHITE]=0x1008000000; 
	
	// Representa o número binário abaixo em hexadecimal
	/*
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 1 0 0 0 0
		0 0 0 0 1 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
	*/
	
	(*board).turn=BLACK;

	return;
}

ulong CoordToBit(int x, int y)
{
	ulong bit = 0x8000000000000000;

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
	
	int shift=8*y+x;

	// Desloca o bit 'shift' (8*y+x) casas para a direita;
	// Fazendo, assim, com que o 1 fique exatamente
	// na coordenada (x,y);

	bit = bit >> shift;

	// Exemplo: Suponha que (x,y) = (3,2),
	// nesse caso a operação bit >> shift resultaria no
	// seguinte número binário:

	/*
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 1 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
	*/

	// Perceba que o 1 que antes estava em (0,0) foi deslocado para (3,2);

	return(bit);
}

ulong GetLegalMoves(BOARD board)
{
	// Essa função obterá um tabuleiro (um inteiro de 64 bits)
	// que conterá as posições dos lances possíveis;
	// Para tal, dividiremos em casos os 3 tipos de captura possíveis:
	//
	// Horizontal, Vertical e Diagonal;
	//
	// Para cada um desses tipos de captura, é criada uma
	// matriz chamada horizontal_board, vertical_board e diagonal_board,
	// respectivamente;
	// 
	// Abaixo pode-se perceber que cada uma será definida como a interseção
	// de onde as peças do inimigo estão localizadas e um tabuleiro inicial
	// para cada um dos casos, como consta abaixo;
	
	ulong horizontal_board=board.playerBoards[1-board.turn] & 0x7e7e7e7e7e7e7e7e;
	
	// Horizontal
	//
	// 0x7e7e7e7e7e7e7e7e
	/*
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
	*/

	ulong vertical_board=board.playerBoards[1-board.turn] & 0x00FFFFFFFFFFFF00;
	// Vertical
	//
	// 0x00FFFFFFFFFFFF00
	/*
		0 0 0 0 0 0 0 0
		1 1 1 1 1 1 1 1
		1 1 1 1 1 1 1 1
		1 1 1 1 1 1 1 1
		1 1 1 1 1 1 1 1
		1 1 1 1 1 1 1 1
		1 1 1 1 1 1 1 1
		0 0 0 0 0 0 0 0
	*/

	ulong diagonal_board=board.playerBoards[1-board.turn] & 0x007e7e7e7e7e7e00;
	
	// Diagonal;
	//
	// 0x007e7e7e7e7e7e00
	/*
		0 0 0 0 0 0 0 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 1 1 1 1 1 1 0
		0 0 0 0 0 0 0 0
	*/

	// Note que onde possui zeros é onde não é possível realizar uma captura no sentido especificado;
	// 
	// Obs: por exemplo, não é possível capturar uma peça adversária pela diagonal se essa está na borda,
	// como pode-se perceber no tabuleiro entitulado 'Diagonal';

	// O critério para se capturar uma peça em Othello é que deve ser possível pular/deslocar uma de suas
	// peças sobre peças do adversário para que essas sejam capturadas. Assim, parece razoável fazer com que
	// o algoritmo abaixo desloque as peças de quem é a vez em todas as direções e procure por uma interseção
	// entre esse deslocamento e as peças do adversário (Pois assim "pulamos" por cima das peças inimigas);

	// Como esse programa utiliza bitboards para a representação do tabuleiro, é bastante simples fazer tal
	// procedimento, basta usar os operadores bitwise de shift >> e << para deslocar o tabuleiro na direção
	// desejada;

	// Algumas observações são necessárias: 
	//
	// Para se deslocar uma casa à direita ou à esquerda, deve se fazer a seguinte operação:
	// tabuleiro << 1 e tabuleiro >> 1, respectivamente;
	// 
	// Para se deslocar uma casa acima ou abaixo, deve se fazer a seguinte operação:
	// tabuleiro << 8 e tabuleiro >> 8, respectivamente;
	//
	// Para se deslocar uma casa na diagonal, note que temos quatro opções:
	// 
	// Nordeste: tabuleiro << 9 ( 8 + 1: Sobe uma linha e avança uma coluna)
	//
	// Noroeste: tabuleiro << 7 ( 8 - 1: Sobe uma linha e volta uma coluna)
	//
	// Sudeste: tabuleiro >> 9 ( 8 + 1: Desce uma linha e avança uma coluna)
	//
	// Sudoeste: tabuleiro >> 7 ( 8 - 1: Desce uma linha e volta uma coluna)

	// Tendo isso em mente, temos o que é necessário para procurar por movimentos válidos,
	// procuremos tais interseções descritas acima e, ainda, construíremos os tabuleiros de
	// captura horizontal, vertical e diagonal.

	// Mas antes, é preciso de definir um tabuleiro de espaços vazios, temporário e o de lances legais:


	// O tabuleiro de espaços vazios é aquele que contém nas casas onde não se tem peças pretas e brancas o número 1;
	// note que esse pode ser obtido por meio de: ~(board.playerBoards[BLACK] | board.playerBoards[WHITE]),
	// a negação de contém preta preta ou contém peça branca.

	ulong blank_board=~(board.playerBoards[BLACK] | board.playerBoards[WHITE]);

	ulong temp_board;

	ulong legal_board;
	
	// Façamos, finalmente, as translações:
	// Obs: Desloca-se o número máximo possível de vezes (6).
	// Como estamos trabalhando com uniões (tmp |= (TRANSLAÇÃO)),
	// não há problema em deslocar um número maior que o necessário,
	// pois caso a translação resulte em um tabuleiro sem peças, ainda
	// sim tmp == tmp | 0;

	temp_board = horizontal_board & (board.playerBoards[board.turn] << 1);
	temp_board |= horizontal_board & (temp_board << 1);
	temp_board |= horizontal_board & (temp_board << 1);
	temp_board |= horizontal_board & (temp_board << 1);
	temp_board |= horizontal_board & (temp_board << 1);
	temp_board |= horizontal_board & (temp_board << 1);
	legal_board = blank_board & (temp_board << 1);

	temp_board = horizontal_board & (board.playerBoards[board.turn] >> 1);
	temp_board |= horizontal_board & (temp_board >> 1);
	temp_board |= horizontal_board & (temp_board >> 1);
	temp_board |= horizontal_board & (temp_board >> 1);
	temp_board |= horizontal_board & (temp_board >> 1);
	temp_board |= horizontal_board & (temp_board >> 1);
	legal_board |= blank_board & (temp_board >> 1);
	
	temp_board = vertical_board & (board.playerBoards[board.turn] << 8);
	temp_board |= vertical_board & (temp_board << 8);
	temp_board |= vertical_board & (temp_board << 8);
	temp_board |= vertical_board & (temp_board << 8);
	temp_board |= vertical_board & (temp_board << 8);
	temp_board |= vertical_board & (temp_board << 8);
	legal_board |= blank_board & (temp_board << 8);

	temp_board = vertical_board & (board.playerBoards[board.turn] >> 8);
	temp_board |= vertical_board & (temp_board >> 8);
	temp_board |= vertical_board & (temp_board >> 8);
	temp_board |= vertical_board & (temp_board >> 8);
	temp_board |= vertical_board & (temp_board >> 8);
	temp_board |= vertical_board & (temp_board >> 8);
	legal_board |= blank_board & (temp_board >> 8);
	
	temp_board = diagonal_board & (board.playerBoards[board.turn] << 9);
	temp_board |= diagonal_board & (temp_board << 9);
	temp_board |= diagonal_board & (temp_board << 9);
	temp_board |= diagonal_board & (temp_board << 9);
	temp_board |= diagonal_board & (temp_board << 9);
	temp_board |= diagonal_board & (temp_board << 9);
	legal_board |= blank_board & (temp_board << 9);

	temp_board = diagonal_board & (board.playerBoards[board.turn] >> 9);
	temp_board |= diagonal_board & (temp_board >> 9);
	temp_board |= diagonal_board & (temp_board >> 9);
	temp_board |= diagonal_board & (temp_board >> 9);
	temp_board |= diagonal_board & (temp_board >> 9);
	temp_board |= diagonal_board & (temp_board >> 9);
	legal_board |= blank_board & (temp_board >> 9);
	
	temp_board = diagonal_board & (board.playerBoards[board.turn] << 7);
	temp_board |= diagonal_board & (temp_board << 7);
	temp_board |= diagonal_board & (temp_board << 7);
	temp_board |= diagonal_board & (temp_board << 7);
	temp_board |= diagonal_board & (temp_board << 7);
	temp_board |= diagonal_board & (temp_board << 7);
	legal_board |= blank_board & (temp_board << 7);
	
	temp_board = diagonal_board & (board.playerBoards[board.turn] >> 7);
	temp_board |= diagonal_board & (temp_board >> 7);
	temp_board |= diagonal_board & (temp_board >> 7);
	temp_board |= diagonal_board & (temp_board >> 7);
	temp_board |= diagonal_board & (temp_board >> 7);
	temp_board |= diagonal_board & (temp_board >> 7);
	legal_board |= blank_board & (temp_board >> 7);

	// Finalmente, retorna o tabuleiro de movimentos legais;

	return(legal_board);
}

bool TurnPassed(BOARD board)
{
	// Copia o tabuleiro atual para o tabuleiro do oponente;
	BOARD opponent_board=board;
	// Passa a vez para o oponente;
	opponent_board.turn=1-opponent_board.turn;
	// Se não há lances possíveis para o tabuleiro atual
	// e há lances possíveis para o tabuleiro do oponente,
	// então a vez deve ser pulada;
	return(GetLegalMoves(board) == 0 && GetLegalMoves(opponent_board) != 0);
}

bool GameFinished(BOARD board)
{
	// Copia o tabuleiro atual para o tabuleiro do oponente;
	BOARD opponent_board=board;
	// Passa a vez para o oponente;
	opponent_board.turn=1-opponent_board.turn;
	// Se não há lances possíveis para o tabuleiro atual
	// e não há lances possíveis para o tabuleiro do oponente,
	// então o jogo deve terminar;
	return(GetLegalMoves(board) == 0 && GetLegalMoves(opponent_board) == 0);
}

ulong ShiftCell(ulong cell_board, int dir)
{
	// Desloca para a direção necessária se 
	// for possível (i.e. a interseção com 
	// o tabuleiro de possibilidades de posições após
	// se deslocar para a direção desejada não é vazia);
	switch (dir)
	{
		case 0:
			// Norte
			return (cell_board << 8) & 0xffffffffffffff00;
			// 0xffffffffffffff00
			/*
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				0 0 0 0 0 0 0 0
			*/
		case 1:
			// Nordeste
			return (cell_board << 7) & 0x7f7f7f7f7f7f7f00;
			// 0x7f7f7f7f7f7f7f00
			/*
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 0 0 0 0 0 0 0
			*/
		case 2:
			// Leste
			return (cell_board >> 1) & 0x7f7f7f7f7f7f7f7f;
			// 0x7f7f7f7f7f7f7f7f
			/*
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
			*/
		case 3:
			// Sudeste
			return (cell_board >> 9) & 0x007f7f7f7f7f7f7f;
			// 0x007f7f7f7f7f7f7f
			/*
				0 0 0 0 0 0 0 0
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
				0 1 1 1 1 1 1 1
			*/
		case 4:
			// Sul
			return (cell_board >> 8) & 0x00ffffffffffffff;
			// 0x00ffffffffffffff
			/*
				0 0 0 0 0 0 0 0
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
				1 1 1 1 1 1 1 1
			*/
		case 5:
			// Sudoeste
			return (cell_board >> 7) & 0x00fefefefefefefe;
			// 0x00fefefefefefefe
			/*
				0 0 0 0 0 0 0 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
			*/
		case 6:
			// Oeste
			return (cell_board << 1) & 0xfefefefefefefefe;
			// 0xfefefefefefefefe
			/*
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
			*/
		case 7:
			// Noroeste
			return (cell_board << 9) & 0xfefefefefefefe00;
			// 0xfefefefefefefe00
			/*
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				1 1 1 1 1 1 1 0
				0 0 0 0 0 0 0 0
			*/
		default:
			// Se não fora possível deslocar
			// a casa, retorna um tabuleiro vazio;
			return(0);
	}
}

void Play(BOARD *board, ulong cell_board)
{
	// Define-se duas bitboards: flipped_board e flipped_temp
	// onde a flipped_board será uma união das flipped_temp
	// e a flipped_temp guardará as peças viradas na
	// direção em que estiver sendo analisada;

	ulong flipped_board=0;
	ulong flipped_temp;

	// Numera-se as direções no sentido
	// horário (de 0 a 7) a partir da casa em que 
	// se deseja jogar da seguinte maneira:

	/*
		[7][0][1]
		[6][X][2]
		[5][4][3]
	*/

	// Para analisar as direções, novamente será feito
	// translações, então é necessário também
	// obter um outro tabuleiro que conterá
	// a casa em que se deseja efetuar o lance
	// deslocada em uma determinada direção;

	ulong shifted_board;

	// Varia-se por todas as direções
	// e troca as peças de lugar

	for (int dir = 0; dir < 8; dir++)
	{
		// Por enquanto nenhuma peça foi virada
		// na direção a ser analisada, então
		// "zera-se" (Considera-se ela igual
		// a célula em que se deseja
		// efetuar o lance) a bitboard flipped_temp;
		
		flipped_temp=cell_board;

		// Desloca-se uma unidade a casa em que se deseja
		// efetuar o lance;
		
		shifted_board=ShiftCell(cell_board, dir);

		// Enquanto for possível fazer tal deslocamento e o deslocamento sobrepor uma peça inimiga,
		// executa o loop abaixo:

		while ((shifted_board != 0) && ((shifted_board & (*board).playerBoards[1-(*board).turn]) != 0))
		{
			// As casa que fora deslocada por sobrepor uma peça inimiga, significa que ela potencialmente
			// será virada, então temporariamente flipped_temp recebe a casa em questão;
			flipped_temp |= shifted_board;
			// Desloca-se novamente a casa e repete o algoritmo;
			shifted_board = ShiftCell(shifted_board, dir);
		}

		// Se encontra-se uma peça aliada, significa que, de fato, o lance era
		// válido e todas aquelas peças que foram sobrepostas devem ser viradas,
		// logo flipped_board recebe as entradas de flipped_temp na direção dir;
		//
		// Note que cell_board está em flipped_temp, assim, a peça em que se
		// deseja efetuar o lance também será virada;

		if ((shifted_board & (*board).playerBoards[(*board).turn]) != 0)
		{
			flipped_board |= flipped_temp;
		}
	}

	// O jogador da vez deve ter posse agora tanto da casa
	// que efetuou o lance quanto das peças que foram viradas (Note que
	// se o lance é válido, cell_board está em flipped_board),
	// sendo assim aplica-se o operador de bitwise OR sobre o tabuleiro
	// do jogador da vez;
	
	(*board).playerBoards[(*board).turn] |= flipped_board;
	
	// É usada o operador de bitwise XOR (ou exclusivo) para
	// tornar as casas em que apresentam 1 tanto no tabuleiro inimigo
	// quanto nas peças viradas em 0, pois o inimigo não
	// deve possuir mais essas peças;
	// Antes, retira-se cell_board de flipped_board usando o operador XOR;

	(*board).playerBoards[1-(*board).turn] ^= (flipped_board ^ cell_board);

	// Troca de quem é a vez de jogar;
	
	(*board).turn = 1-(*board).turn;
	
	return;
}

int DiskCount(ulong board)
{
	// De início, o número de discos é nulo;

	int num_disks=0;
	
	// Define-se uma bitboard de contagem, onde
	// a única casa que possui um valor não nulo
	// será variada, percorrendo assim toda a bitboard;
	//
	// Obs: De início a casa não nula parte do canto superior
	// esquerdo;

	ulong counter_board=0x8000000000000000;
	// 0x8000000000000000
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

	// Variando sobre as 64 casas do tabuleiro;

	for (int k = 0; k < 64; k++)
	{
		// Se existe uma interseção entre 
		// o tabuleiro e a bitboard de contagem, 
		// então existe uma peça no tabuleiro na
		// casa em questão, logo aumenta o número
		// de discos;

		if ((counter_board & board) != 0)
		{
			num_disks+=1;
		}
		
		// Desloca a casa não nula da bitboard de
		// contagem para a direita;
		counter_board=counter_board >> 1;
	}

	// Retorna a quantidade de discos;

	return (num_disks);
}

ulong *GetCoords(ulong board, int *num_disks)
{
	// Define uma tabela de pesos para posteriormente ordenar
	// os movimentos possíveis
	
	int weights_matrix[64] =
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

	// Obtém o número de discos no tabuleiro;

	(*num_disks)=DiskCount(board);
	
	// Define também um vetor para salvar
	// esses pesos
	
	int pos_evals[(*num_disks)];

	// Aloca o vetor que conterá as posições dos discos;

	ulong *positions = (ulong *)malloc((*num_disks)*sizeof(ulong));
	
	// Define-se a variável index para especificar
	// onde será guardada uma determinada posição
	// no vetor;
	//
	// Obs: De início, guarda-se na posição 0;
	
	int index=0;

	// Define-se uma bitboard de contagem, onde
	// a única casa que possui um valor não nulo
	// será variada, percorrendo assim toda a bitboard;
	//
	// Obs: De início a casa não nula parte do canto superior
	// esquerdo;

	ulong counter_board=0x8000000000000000;
	// 0x8000000000000000
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

	// Variando sobre as 64 casas do tabuleiro;

	for (int k = 0; k < 64; k++)
	{
		// Se existe uma interseção entre 
		// o tabuleiro e a bitboard de contagem, 
		// então existe uma peça no tabuleiro na
		// casa em questão, logo guarda a posição
		// no vetor positions, no índice index;

		if ((counter_board & board) != 0)
		{
			positions[index]=counter_board;
			pos_evals[index]=weights_matrix[index];
			index++;
		}
		
		// Desloca a casa não nula da bitboard de
		// contagem para a direita;
		counter_board=counter_board >> 1;
	}

	// Aplica bubble sort no vetor pos_evals
	// e reordena os elementos de positions;
	
	// Define um tabuleiro temporário que guarda a posição de um lance
	// para que seja possível realizar o bubble sort
	
	ulong temp;
	
	for (int i = 0; i < *num_disks-1; i++)
	{
		for (int j = i; j < *num_disks-1; j++)
		{
			if (pos_evals[j] < pos_evals[j+1])
			{
				temp=positions[j];
				positions[j]=positions[j+1];
				positions[j+1]=temp;
			}
		}
	}

	// Retorna o vetor de posições;

	return (positions);
}
