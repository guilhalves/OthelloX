#include "../include/core.h"
#include "../include/screen.h"
#include "../include/intelligence.h"

int main()
{
	// Define algumas configurações:
	// player: Quem o humano será, caso for humano x máquina;
	// daltonism: Indica se a pessoa possui ou não daltonismo;
	// player_vs_player: Indica se é humano x humano ou humano x máquina;
	// restart: Indica se o jogador deseja recomeçar a partida;
	int player;
	int daltonism;
	int player_vs_player;
	int restart;

	// Cria o tabuleiro;
	
	BOARD board;
	
	// Pergunta ao usuário qual tipo de jogo
	// ele deseja jogar;
	
	do {
		printf("\nDESEJA JOGAR CONTRA QUAL OPONENTE?\n\n[0] PEDRO (MÁQUINA)\n\n[1] OUTRO JOGADOR\n\n");
		scanf("%d", &player_vs_player);
	} while (player_vs_player != 0 && player_vs_player != 1);

	// Se for máquina contra jogador, pergunta qual peça o jogador terá;
	
	if (!player_vs_player)
	{
		// Pergunta ao usuário qual peça ele quer possuir;
		
		printf("\nDESEJA POSSUIR OS DISCOS PRETOS OU BRANCOS?\n\n[0] PRETAS\n\n[1] BRANCAS\n\n");
		scanf("%d", &player);
	}
	
	// Pergunta ao usuário se ele tem daltonismo;

	do {
		printf("\nVOCÊ POSSUI DALTONISMO?\n\n[0] NÃO\n\n[1] SIM\n\n");
		scanf("%d", &daltonism);
	} while (daltonism != 0 && daltonism != 1);

	// Se o usuário possui daltonismo, pergunta qual tipo de daltonismo
	// se trata, para a devida correção das cores;

	if (daltonism == 1)
	{
		do {
			printf("\nSELECIONE O SEU TIPO DE DALTONISMO:\n\n[1] ACROMATOPSIA\n\n[2] ACROMATOPSIA ATÍPICA\n\n[3] PROTANOPIA\n\n[4] DEUTERANOPIA\n\n[5] TRITANOPIA\n\n[6] PROTANOMALIA\n\n[7] DEUTERANOMALIA\n\n[8] TRITANOMALIA\n\n");
			scanf("%d", &daltonism);
		} while (daltonism < 1 || daltonism > 8);
	}

	// Inicia a janela do jogo;
	
	do {

		// Reseta o tabuleiro;
		ResetBoard(&board);
		
		// Inicia o jogo;
		InitGame(&board, player, daltonism, player_vs_player);
		
		// Após o término do jogo, pergunta ao usuário se ele deseja recomeçar;
		do {
			printf("\nDESEJA INICIAR OUTRA PARTIDA?\n\n[0] NÃO\n\n[1] SIM\n\n");
			scanf("%d", &restart);
		} while (restart != 0 && restart != 1);
	
	} while (restart != 0);

	return(0);
}
