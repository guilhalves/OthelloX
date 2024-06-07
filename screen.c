#include "screen.h"

void DrawCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius)
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
				SDL_RenderDrawPoint(renderer, center_x+x, center_y+y);
			}
		}		
	}
	return;
}

void CreateWindow(BOARD *board) {

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

	int board_size=640;

	int margin=8;

	bool running = 1;
	
	SDL_Rect rect;

	ulong legal_board;

	ulong cell_board;

	clock_t initial_time;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Cria a janela;

	SDL_Window *window = SDL_CreateWindow("Othello-X", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, board_size, board_size, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Cria o renderizador; 

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Define uma variável do tipo evento;

	SDL_Event event;

	// Define uma variável do tipo evento do mouse;

	SDL_MouseButtonEvent *mouse = &event.button;

	// Enquanto estiver rodando (running == true), executa o código abaixo

	while (running)
	{
		// Define a cor de fundo como um verde escuro;

		SDL_SetRenderDrawColor(renderer, 9, 51, 0, 255);

		// Preenche a tela com a cor acima;

		SDL_RenderClear(renderer);
		
		// Define a cor de desenho como um verde claro;

		SDL_SetRenderDrawColor(renderer, 0, 153, 51, 255);

		// Define o comprimento (board_size) e altura (board_size) do quadrado;

		rect.w = (board_size/8)-margin/2.0;
		rect.h = (board_size/8)-margin/2.0;

		// Desenha 64 quadrados para formar um tabuleiro na tela com a cor
		// especificada;

		for (int y = margin/4.0; y < board_size; y+=board_size/8)
		{

			// Define a posição vertical do quadrado;

			rect.y = y;

			for (int x = margin/4.0; x < board_size; x+=board_size/8)
			{
				// Define a posição horizontal do quadrado;

				rect.x = x;

				// Desenha o quadrado;

				SDL_RenderFillRect(renderer, &rect);
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
				DrawCircle(renderer, (board_size/8.0)*(i / 8 + 1/2.0), (board_size/8.0)*(i % 8 + 1/2.0), (board_size/(16.0)-margin));
			}
			if ((CoordToBit(i / 8, i % 8) & (*board).playerBoards[WHITE]) != 0)
			{

				// Se existe uma peça branca na casa em questão (Interseção não vazia), 
				// seta a cor de desenho do renderizador para branco e desenha uma peça
				// branca no tabuleiro;
			
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				DrawCircle(renderer, (board_size/8.0)*(i / 8 + 1/2.0), (board_size/8.0)*(i % 8 + 1/2.0), (board_size/(16.0)-margin));
			}
			if ((CoordToBit(i / 8, i % 8) & legal_board) != 0)
			{

				// Se existe interseção entre a casa em questão e a bitboard de lances
				// legais, seta a cor de desenho do renderizador para cinza e desenha uma bolinha
				// cinza no tabuleiro para indicar que ali existe um lance possível;

				SDL_SetRenderDrawColor(renderer, 255, 51, 51, 255);
				DrawCircle(renderer, (board_size/8.0)*(i / 8 + 1/2.0), (board_size/8.0)*(i % 8 + 1/2.0), (board_size/(32.0)-margin));
			}
		}

		// Mostra o que foi desenhado;

		SDL_RenderPresent(renderer);

		// Se for a vez da máquina, procura o melhor lance e o efetua;

		if (!GameFinished(*board))
		{
			if ((*board).turn == WHITE)
			{
				initial_time=clock();
				cell_board=SearchBest(*board, initial_time);
				Play(board, cell_board);
			}
		} else
		{
			printf("\nO: %d, X: %d\n", DiskCount((*board).playerBoards[0], 0), DiskCount((*board).playerBoards[1], 1));
			ResetBoard(board);
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

						// Se for a vez do jogador, segue.

						if ((*board).turn == BLACK)
						{
							// Transcreve as coordenadas da janela
							// para o tabuleiro e executa um lance
							// se for a vez do usuário e o lance for
							// legal;

							// Obtém o tabuleiro que contém a casa
							// em que se deseja efetuar o lance;
					
							cell_board=CoordToBit((*mouse).x / (board_size/8), (*mouse).y / (board_size/8));
	
							// Se é um lance válido, executa o lance; 
							// Isto é, há interseção com o tabuleiro
							// de lances válidos;
	
							if ((cell_board & GetLegalMoves(*board)) != 0)
							{
								Play(board, cell_board);
							}
						}
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
						SDL_SetWindowSize(window, board_size, board_size);
						margin=8*(board_size/640.0);
					}
				}
			}
		}
	}

	// Destrói o renderizador e a janela, e depois finaliza
	// o SDL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return;
}
