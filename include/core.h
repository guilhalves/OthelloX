#pragma once
#include <stdio.h>
#include <stdbool.h>

// IMPORTANTE
//
// Ás vezes o par ordenado (x,y)
// representa a linha y e coluna x
// e às vezes a linha x e coluna y;
//
// Nos comentários do código eles representarão
// a linha y e coluna x, enquanto nos printf's
// serão o oposto.
//
// Essa distinção serve apenas para facilitar
// a leitura, sendo que nos printf's mostra-se 
//a notação usual.

// Para representar de quem será
// a vez, será utilizado uma variável
// booleana. Abaixo define-se qual jogador
// recebe qual estado;

#define BLACK 0
#define WHITE 1

// Para poupar esforço, unsigned long int
// passa a ser chamado de ulong;

typedef unsigned long int ulong;

// Define-se uma struct BOARD
// que conterá as informações do
// tabuleiro, como duas bitboards (ulong)
// que serão os tabuleiros das pretas e brancas
// e de quem é a vez naquele tabuleiro;
// Obs: Utiliza-se um vetor para criar as duas
// bitboards pois é conveniente. Exemplo,
// pode-se obter o tabuleiro de quem é a vez
// simplesmente por meio de board.playerBoards[turn],
// já que turn é booleano;

typedef struct _BOARD {
	ulong playerBoards[2];
	bool turn;
} BOARD;

// A função ResetBoard faz com que
// o tabuleiro volte a configuração
// inicial;

void ResetBoard(BOARD *board);

// A função CoordToBit pega um par
// ordenado (x, y) e cria sua
// respectiva bitboard;

ulong CoordToBit(int x, int y);

// A função GetLegalMoves obtém a
// bitboard com 1's nas entradas
// onde é possível efetuar um lance;

ulong GetLegalMoves(BOARD board);

// A função TurnPassed verifica se o
// jogador da vez não possui lances
// disponíveis;

bool TurnPassed(BOARD board);

// A função GameFinished verifica
// se o jogo chegou ao fim,
// analisando se a vez foi passada
// 2 vezes;

bool GameFinished(BOARD board);

// A função ShiftCell desloca as peças de uma
// bitboard em uma determinada direção;

ulong ShiftCell(ulong cell_board, int dir);

// A função Play efetua um lance na casa
// desejada;

void Play(BOARD *board, ulong cell_board);

// A função DiskCount calcula quantas peças
// tem no tabuleiro;

int DiskCount(ulong board);

// A função GetCoords obtém o endereço
// de um array contendo as posições
// dos discos em um tabuleiro;

ulong *GetCoords(ulong board, int *num_moves);
