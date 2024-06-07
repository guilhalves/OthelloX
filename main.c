#include "core.h"
#include "screen.h"

int main()
{
	// Cria o tabuleiro;
	
	BOARD board;
	
	// Reseta o tabuleiro;

	ResetBoard(&board);
	
	// Inicia a janela do jogo;
	
	CreateWindow(&board);

	return(0);
}
