#include "../include/screen.h"

void DrawCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius, bool transparent)
{
	// Desenha um círculo, variando x e y em torno
	// do centro. Para verificar se um ponto deve
	// ou não ser pintado, ele deve satisfazer
	// a seguinte equação reduzida do círculo:
	// x^2 + y^2 <= radius^2;
	for (int x = -radius; x <= radius; x++)
	{
		for(int y = -radius; y <= radius; y++) 
		{
			if ((x*x + y*y) <= radius*radius)
			{
				// Se o ponto satisfaz a equação reduzida
				// do círculo, então pinta o ponto na tela;

				// Verifica também se é para ele ser transparente.
				// Se sim, pinta somente metades dos pixels em um
				// padrão diagonal;
				if (transparent)
				{
					if ((x+y)%2)
					{
						SDL_RenderDrawPoint(renderer, center_x+x, center_y+y);
					}
				} else
				{
					SDL_RenderDrawPoint(renderer, center_x+x, center_y+y);
				}
			}
		}		
	}
	return;
}

void InitGame(BOARD *board, int player, int daltonism, int player_vs_player) {

	// Declarando variáveis:
	//
	// board_size: representa o tamanho em pixels
	// do lado do tabuleiro;
	//
	// margin: Especifica o tamanho de uma margem entre
	// objetos na tela;
	//
	// running: É usado como condição de parada
	// para a janela;
	//
	// rect: É um retângulo usado para desenhar o tabuleiro;
	//
	// legal_board: Tabuleiro de lances possíveis que é atualizado
	// a cada lance;
	//
	// cell_board: Tabuleiro que contém a casa em que o jogador
	// clicar com o mouse na janela;
	//
	// initial_time: Guarda o tempo em que se iniciou a calcular um lance;
	//
	// zobrist_table: Tabela com valores aleatórios para a geração de hashs;
	//
	// hash_table: Tabela de transposição (também conhecida como hash table),
	// que guarda lances previamente calculados;
	//
	// num_disks: Guarda o número de discos no tabuleiro atual;
	//
	// move_history: Guarda os lances que foram realizados na partida.
	//
	// eval_history: Guarda quem estava na vantagem em diferente estágios
	// do jogo;

	int board_size=640;

	int margin=8;

	bool running = 1;
	
	SDL_Rect rect;

	ulong legal_board;

	ulong cell_board=0;

	int num_disks;

	int eval_history[60] = {0};
	
	int move_history[60][2];
	
	clock_t initial_time;

	ulong zobrist_table[128];

	// Inicializa os valores da zobrist_table;

	InitHashTable(zobrist_table);

	// Cria a hash table;

	TRANSPOSITION *hash_table = CreateHashTable();

	// Inicializa o SDL;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Cria a janela;

	SDL_Window *window = SDL_CreateWindow("Othello-X", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 7*board_size/4.0, board_size, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Cria o renderizador; 

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Carrega o ícone;

	SDL_Surface *icon = IMG_Load("icon.png");
	if (icon == NULL)
	{
		printf("NÃO FOI POSSÍVEL CARREGAR O ÍCONE\n");
	} else
	{
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}

	// Define uma variável do tipo evento;

	SDL_Event event;
	
	// Define uma variável do tipo evento do mouse;

	SDL_MouseButtonEvent *mouse = &event.button;

	// Enquanto estiver rodando (running == true), executa o código abaixo

	while (running)
	{
		// Calcula quantos discos existem no tabuleiro;

		num_disks=DiskCount((*board).playerBoards[0])+DiskCount((*board).playerBoards[1]);
		
		// Define a cor de fundo;
		
		switch (daltonism)
		{
			case NAO_DALTONICO:
				SDL_SetRenderDrawColor(renderer, 9, 51, 0, 255);
				break;
			case ACROMATOPSIA:
				SDL_SetRenderDrawColor(renderer, 38, 38, 38, 255);
				break;
			case ACROMATOPSIA_AT:
				SDL_SetRenderDrawColor(renderer, 28, 43, 24, 255);
				break;
			case PROTANOPIA:
				SDL_SetRenderDrawColor(renderer, 53, 47, 0, 255);
				break;
			case DEUTERANOPIA:
				SDL_SetRenderDrawColor(renderer, 60, 45, 12, 255);
				break;
			case TRITANOPIA:
				SDL_SetRenderDrawColor(renderer, 29, 50, 54, 255);
				break;
			case PROTANOMALIA:
				SDL_SetRenderDrawColor(renderer, 37, 49, 0, 255);
				break;
			case DEUTERANOMALIA:
				SDL_SetRenderDrawColor(renderer, 41, 47, 8, 255);
				break;
			case TRITANOMALIA:
				SDL_SetRenderDrawColor(renderer, 22, 51, 35, 255);
				break;
		}

		// Preenche a tela com a cor acima;

		SDL_RenderClear(renderer);
		
		// Define o comprimento (board_size) e altura (board_size) do quadrado;

		rect.w = (board_size/8)-margin/2.0;
		rect.h = (board_size/8)-margin/2.0;

		// Desenha 64 quadrados para formar um tabuleiro na tela com a cor
		// especificada;

		for (int i = 0, y = margin/4.0; y < 8*(board_size/8); y+=board_size/8)
		{

			// Define a posição vertical do quadrado;

			rect.y = y;

			for (int x = margin/4.0; x < 8*(board_size/8); x+=board_size/8)
			{
				// Define a posição horizontal do quadrado;

				rect.x = x;

				// Desenha o quadrado;

				// Se a i-ésima casa possuir interseção
				// com a casa do último lance, 
				// pinta de vermelho (ou o análogo para os daltônicos);

				if ((CoordToBit(i % 8, i / 8) & cell_board) != 0)
				{
					switch (daltonism)
					{
						case NAO_DALTONICO:
							SDL_SetRenderDrawColor(renderer, 255, 51, 51, 255);
							break;
						case ACROMATOPSIA:
							SDL_SetRenderDrawColor(renderer, 94, 94, 94, 255);
							break;
						case ACROMATOPSIA_AT:
							SDL_SetRenderDrawColor(renderer, 153, 79, 79, 255);
							break;
						case PROTANOPIA:
							SDL_SetRenderDrawColor(renderer, 147, 133, 74, 255);
							break;
						case DEUTERANOPIA:
							SDL_SetRenderDrawColor(renderer, 169, 126, 37, 255);
							break;
						case TRITANOPIA:
							SDL_SetRenderDrawColor(renderer, 255, 54, 55, 255);
							break;
						case PROTANOMALIA:
							SDL_SetRenderDrawColor(renderer, 186, 103, 66, 255);
							break;
						case DEUTERANOMALIA:
							SDL_SetRenderDrawColor(renderer, 200, 99, 42, 255);
							break;
						case TRITANOMALIA:
							SDL_SetRenderDrawColor(renderer, 255, 53, 53, 255);
							break;
					}
				} else
				{
					// Se não, pinta de verde, ou o análogo para
					// os daltônicos;

					switch (daltonism)
					{
						case NAO_DALTONICO:
						SDL_SetRenderDrawColor(renderer, 0, 153, 51, 255);
							break;
						case ACROMATOPSIA:
							SDL_SetRenderDrawColor(renderer, 113, 113, 113, 255);
							break;
						case ACROMATOPSIA_AT:
							SDL_SetRenderDrawColor(renderer, 72, 128, 91, 255);
							break;
						case PROTANOPIA:
							SDL_SetRenderDrawColor(renderer, 147, 131, 49, 255);
							break;
						case DEUTERANOPIA:
							SDL_SetRenderDrawColor(renderer, 163, 124, 65, 255);
							break;
						case TRITANOPIA:
							SDL_SetRenderDrawColor(renderer, 66, 142, 153, 255);
							break;
						case PROTANOMALIA:
							SDL_SetRenderDrawColor(renderer, 93, 139, 50, 255);
							break;
						case DEUTERANOMALIA:
							SDL_SetRenderDrawColor(renderer, 103, 135, 60, 255);
							break;
						case TRITANOMALIA:
							SDL_SetRenderDrawColor(renderer, 42, 146, 116, 255);
							break;
					}
				}
				
				// Pinta o quadrado;

				SDL_RenderFillRect(renderer, &rect);
				
				// Aumenta o índice da casa;

				i++;
			}
		}

		// Se não existe movimento (Vez pulada),
		// troca de quem é a vez de jogar;

		if (TurnPassed(*board))
		{
			(*board).turn=1-(*board).turn;
		}

		// Obtém a bitboard de movimentos válidos;

		legal_board=GetLegalMoves(*board);
			
		// Analisa bit a bit da bitboard e desenha as peças
		// no tabuleiro em suas devidas posições, se existirem;

		// Varia-se por todas as 64 casas do tabuleiro;

		for (int i = 0; i < 64; i++)
		{
			if ((CoordToBit(i / 8, i % 8) & (*board).playerBoards[BLACK]) != 0)
			{

				// Se existe uma peça preta na casa em questão (Interseção não vazia), 
				// seta a cor de desenho do renderizador para preto e desenha uma peça
				// preta no tabuleiro;
			
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				DrawCircle(renderer, (board_size/8.0)*(i / 8 + 1/2.0), (board_size/8.0)*(i % 8 + 1/2.0), (board_size/(16.0)-margin), 0);
			}
			if ((CoordToBit(i / 8, i % 8) & (*board).playerBoards[WHITE]) != 0)
			{

				// Se existe uma peça branca na casa em questão (Interseção não vazia), 
				// seta a cor de desenho do renderizador para branco e desenha uma peça
				// branca no tabuleiro;
			
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				DrawCircle(renderer, (board_size/8.0)*(i / 8 + 1/2.0), (board_size/8.0)*(i % 8 + 1/2.0), (board_size/(16.0)-margin), 0);
			}
			if ((CoordToBit(i / 8, i % 8) & legal_board) != 0)
			{

				// Se existe interseção entre a casa em questão e a bitboard de lances
				// legais, seta a cor de desenho do renderizador para cinza e desenha uma bolinha
				// cinza no tabuleiro para indicar que ali existe um lance possível;

				SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
				DrawCircle(renderer, (board_size/8.0)*(i / 8 + 1/2.0), (board_size/8.0)*(i % 8 + 1/2.0), (board_size/(24.0)-margin), 1);
			}

		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		
		// Desenha o gráfico de vantagem

		for (int i = 0; i < num_disks-5; i++)
		{
			SDL_RenderDrawLine(renderer, board_size+margin*36, (board_size/2)-100, board_size+margin*36, (board_size/2)+100);
			SDL_RenderDrawLine(renderer, board_size, board_size/2, board_size+margin*(58), board_size/2);
			SDL_RenderDrawLine(renderer, board_size+margin*i, board_size/2-eval_history[i]/400, board_size+margin*(i+1), board_size/2-eval_history[i+1]/400);
		}
		
		// Mostra o que foi desenhado;

		SDL_RenderPresent(renderer);

		// Se o jogo não acabou, procura por um lance
		// se for humano x máquina;

		if (!GameFinished(*board))
		{
			// Se não é humano x humando, segue.
			
			if (!(player_vs_player))
			{
				// Se a vez for do computador, segue;

				if ((*board).turn != player)
				{
					// Guarda de quem é a vantagem no tabuleiro atualmente;

					eval_history[num_disks-4]=-EvalFunction(*board);
					
					// Salva o tempo atual ao iniciar o
					// cálculo de um lance, para verificar
					// se o cálculo está respeitando o tempo
					// limite;
					
					initial_time=clock();
					
					// Procura-se pelo melhor lance;
						
					cell_board=SearchBest(*board, initial_time, zobrist_table, hash_table);
					
					// Efetua o lance;
					
					Play(board, cell_board);
					
					// Mostra o placar atual

					printf("\nPRETAS: %d\nBRANCAS: %d\n\n", DiskCount((*board).playerBoards[0]), DiskCount((*board).playerBoards[1]));
					// Salva o lance no histórico da partida

					for (int i = 0; i < 64; i++)
					{
						if ((CoordToBit(i % 8, i / 8) & cell_board) != 0)
						{
							move_history[num_disks-4][0]=i/8;
							move_history[num_disks-4][1]=i%8;
						}
					}

				}
				else
				{
					// Guarda de quem é a vantagem no tabuleiro atualmente;
					
					eval_history[num_disks-4]=EvalFunction(*board);	
					
					// Salva o lance no histórico da partida

					for (int i = 0; i < 64; i++)
					{
						if ((CoordToBit(i % 8, i / 8) & cell_board) != 0)
						{
							move_history[num_disks-4][0]=i/8;
							move_history[num_disks-4][1]=i%8;
						}
					}
				}
			}
			else
			{
				// Se for humano x humano, salva
				// de quem é a vantagem no tabuleiro atualmente;

				if ((*board).turn == WHITE)
				{
					eval_history[num_disks-4]=-EvalFunction(*board);
				} else
				{
					eval_history[num_disks-4]=EvalFunction(*board);
				}
			}
		}
		
		// Verifica se o usuário interagiu com a
		// interface de alguma maneira;

		while(SDL_PollEvent(&event)) 
		{
			switch (event.type)
			{

				// Se aperta o botão de fechar, termina o programa;

				case SDL_QUIT:
					{
						running = 0;
						break;
					}

					// Se clica com o botão esquerdo do mouse, tenta
					// efetuar um lance;

				case SDL_MOUSEBUTTONDOWN:
					{
						if ((*mouse).button == SDL_BUTTON_LEFT)
						{

							// Transcreve as coordenadas da janela
							// para o tabuleiro e executa um lance
							// se for a vez do usuário e o lance for
							// legal;

							// Obtém o tabuleiro que contém a casa
							// em que se deseja efetuar o lance;
							// (Se estiver dentro do tabuleiro)
							if ((*mouse).x <= board_size && (*mouse).y <= board_size)
							{
								cell_board=CoordToBit((*mouse).x / (board_size/8), (*mouse).y / (board_size/8));
							} else
							{
								break;
							}
							// Se for humano x máquina, entra no if;

							if (!(player_vs_player))
							{
								// Se for a vez do jogador, segue.

								if ((*board).turn == player)
								{	
									// Se é um lance válido, executa o lance; 
									// Isto é, há interseção com o tabuleiro
									// de lances válidos;
									if ((cell_board & GetLegalMoves(*board)) != 0)
									{
										Play(board, cell_board);

										// Salva o lance no histórico da partida;

										move_history[num_disks-4][0]=(*mouse).y/(board_size/8);
										move_history[num_disks-4][1]=(*mouse).x/(board_size/8);
										
										// Mostra o placar atual
										
										printf("\nPRETAS: %d\nBRANCAS: %d\n\n", DiskCount((*board).playerBoards[0]), DiskCount((*board).playerBoards[1]));
									}
								}
							} else
							{
								// Se for humano x humano, simplesmente efetua o lance se ele for válido;

								if ((cell_board & GetLegalMoves(*board)) != 0)
								{
									Play(board, cell_board);
									
									// Salva o lance no histórico da partida;
									
									move_history[num_disks-4][0]=(*mouse).y/(board_size/8);
									move_history[num_disks-4][1]=(*mouse).x/(board_size/8);
									
									// Mostra o placar atual
									
									printf("\nPRETAS: %d\nBRANCAS: %d\n\n", DiskCount((*board).playerBoards[0]), DiskCount((*board).playerBoards[1]));
								}

							}
						} else if ((*mouse).button == SDL_BUTTON_RIGHT)
						{
							// Se o jogador clicar com o botão direito, apenas destaca a casa
							// em questão

							cell_board=CoordToBit((*mouse).x / (board_size/8), (*mouse).y / (board_size/8));
						}
						break;
					}

					// Se a janela for redimensionada, converte seu tamanho
					// mantendo a proporção;

				case SDL_WINDOWEVENT:
					{
						if(event.window.event == SDL_WINDOWEVENT_RESIZED)
						{

							// Obtém a altura após redimensionar a tela e
							// define essa altura em pixels como sendo o
							// tamanho do tabuleiro;

							board_size=event.window.data2;

							// Define um tamanho mínimo para a janela, se for
							// menor que o mínimo passa para o mínimo;

							if (board_size < 320) {
								board_size=320;
							}

							// Redimensiona a janela e atualiza o tamanho da margem;
							SDL_SetWindowSize(window, 7*board_size/4.0, board_size);
							margin=8*(board_size/640.0);
						}
					}
			}
		}
	}
	
	// Destrói o renderizador e a janela, e depois finaliza
	// o SDL;

	free(hash_table);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	// Pergunta ao usuário se ele deseja exportar a partida;
	
	int export;
	
	do {
		printf("DESEJA EXPORTAR A PARTIDA?\n\n[0] NÃO\n\n[1] SIM\n\n");
		scanf("%d", &export);
	} while (export != 1 && export != 0);

	// Se sim, mostra os lances executados;
	
	if (export)
	{
		printf("(%d,%d)", move_history[0][0]+1, move_history[0][1]+1);
		for (int i = 1; i < num_disks-4; i++)
		{
			printf(";(%d,%d)", move_history[i][0]+1, move_history[i][1]+1);
		}
		printf("\n\n");
	}

	return;
}
