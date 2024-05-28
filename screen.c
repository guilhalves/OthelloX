#include "screen.h"

int board_size=640;
int margin=8;

void DrawUpLine(SDL_Renderer *renderer, SDL_Rect h_rect, int center_x, int center_y, int size)
{
	h_rect.x=center_x-size/2.0;
	h_rect.y=center_y-size/2.0;
	SDL_RenderFillRect(renderer, &h_rect);
	return;
}
void DrawMidLine(SDL_Renderer *renderer, SDL_Rect h_rect, int center_x, int center_y, int size)
{
	h_rect.x=center_x-size/2.0;
	h_rect.y=center_y-margin/4.0;
	SDL_RenderFillRect(renderer, &h_rect);
	return;
}
void DrawDownLine(SDL_Renderer *renderer, SDL_Rect h_rect, int center_x, int center_y, int size)
{
	h_rect.x=center_x-size/2.0;
	h_rect.y=center_y+size/2.0-margin/2.0;
	SDL_RenderFillRect(renderer, &h_rect);
	return;
}
void DrawLeftUpLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size)
{
	v_rect.x=center_x-size/2.0;
	v_rect.y=center_y-size/2.0;
	SDL_RenderFillRect(renderer, &v_rect);
	return;
}
void DrawLeftDownLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size)
{
	v_rect.x=center_x-size/2.0;
	v_rect.y=center_y;
	SDL_RenderFillRect(renderer, &v_rect);
	return;
}
void DrawRightUpLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size)
{
	v_rect.x=center_x+size/2.0-margin/2.0;
	v_rect.y=center_y-size/2.0;
	SDL_RenderFillRect(renderer, &v_rect);
	return;
}
void DrawRightDownLine(SDL_Renderer *renderer, SDL_Rect v_rect, int center_x, int center_y, int size)
{
	v_rect.x=center_x+size/2.0-margin/2.0;
	v_rect.y=center_y;
	SDL_RenderFillRect(renderer, &v_rect);
	return;
}

void DrawDigit(SDL_Renderer *renderer, int center_x, int center_y, int size, int digit) {
	
	SDL_Rect h_rect;
	
	SDL_Rect v_rect;

	h_rect.w=size;
	h_rect.h=margin/2.0;
	v_rect.h=size/2.0;
	v_rect.w=margin/2.0;

	switch (digit)
	{
		case 0:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftUpLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 1:
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 2:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 3:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 4:
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftUpLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 5:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftUpLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 6:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftUpLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 7:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 8:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftUpLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftDownLine(renderer, v_rect, center_x, center_y, size);
			break;
		case 9:
			DrawUpLine(renderer, h_rect, center_x, center_y, size);
			DrawMidLine(renderer, h_rect, center_x, center_y, size);
			DrawDownLine(renderer, h_rect, center_x, center_y, size);
			DrawRightUpLine(renderer, v_rect, center_x, center_y, size);
			DrawRightDownLine(renderer, v_rect, center_x, center_y, size);
			DrawLeftUpLine(renderer, v_rect, center_x, center_y, size);
			break;
	}
	return;
}

void DrawNumber(SDL_Renderer *renderer, int center_x, int center_y, int size, int number)
{
	if (number / 10 != 0)
	{
		DrawDigit(renderer, center_x-2*size+margin, center_y, size, number / 10);
		DrawDigit(renderer, center_x, center_y, size, number % 10);
		return;
	}
	DrawDigit(renderer, center_x, center_y, size, number % 10);
	return;
}

void DrawCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius)
{
	int dx;
	int dy;
	for (int x = 0; x <= radius*2; x++)
	{
		for(int y = 0; y <= radius*2; y++) 
		{
			dx = radius - x;
			dy = radius - y;
			if ((dx*dx + dy*dy) <= radius*radius)
			{
				SDL_RenderDrawPoint(renderer, center_x+dx, center_y+dy);
			}
		}		
	}
	return;
}

void DrawHollowCircle(SDL_Renderer *renderer, int center_x, int center_y, int radius)
{
	DrawCircle(renderer, center_x, center_y, radius);

	// Define a cor de desenho do renderizador como sendo a cor
	// de fundo:

	// Cinza esverdeado;

	SDL_SetRenderDrawColor(renderer, 35, 43, 43, 255);

	DrawCircle(renderer, center_x, center_y, radius-margin);
	return;
}

void DrawCross(SDL_Renderer *renderer, int center_x, int center_y, int length) {

	for (int x = -length/2; x <= length/2; x++)
	{
		DrawCircle(renderer, center_x+x, center_y+x, margin/2);
		DrawCircle(renderer, center_x-x, center_y+x, margin/2);
	}
	return;
}

void DrawSquare(SDL_Renderer *renderer, int center_x, int center_y, int length)
{
	SDL_Rect rect;

	rect.w = 3*length/2.0;
	rect.h = 3*length/2.0;

	rect.x = center_x-3*length/4.0;
	rect.y = center_y-3*length/4.0;

	SDL_RenderFillRect(renderer, &rect);

	return;
}

void ShowMenu(SDL_Window *window, SDL_Renderer *renderer, NODE *node) {
	
	SDL_Rect rect;
	
	rect.x = board_size+margin/2.0;
	rect.y = margin/2.0;

	rect.h = board_size-margin;
	rect.w = board_size/2.0-margin;

	SDL_SetRenderDrawColor(renderer, 35, 43, 43, 255);
	
	SDL_RenderFillRect(renderer, &rect);

	// Desenha O e X
	SDL_SetRenderDrawColor(renderer, 0, 203, 204, 255);
	DrawHollowCircle(renderer, 5*board_size/16+board_size, board_size/8, board_size/(2*SIZE)*(3.0/SIZE));
	SDL_SetRenderDrawColor(renderer, 255, 5, 52, 255);
	DrawCross(renderer, 5*board_size/16+board_size, board_size/8+4*(board_size/(2*SIZE)-2*margin), (board_size/(2*SIZE)-2*margin));

	// Desenha o placar;
	SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255);
	DrawNumber(renderer, board_size+3*board_size/16, board_size/8, (board_size/(2*SIZE)-2*margin), (*node).num_disks[CIRCLE]);
	DrawNumber(renderer, board_size+3*board_size/16, board_size/8+4*(board_size/(2*SIZE)-2*margin), (board_size/(2*SIZE)-2*margin), (*node).num_disks[CROSS]);

	// Desenha botão de restart;

	SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255);
	
	DrawCircle(renderer, 5*board_size/4.0, 3*board_size/4.00, board_size/16.0+margin);

	SDL_SetRenderDrawColor(renderer, 35, 43, 43, 255);

	DrawCircle(renderer, 5*board_size/4.0, 3*board_size/4.0, board_size/16.0-2*margin);
	
	SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255);

	DrawCircle(renderer, 5*board_size/4.0, 3*board_size/4.0, 3*board_size/64.0-2*margin);
	
	SDL_SetRenderDrawColor(renderer, 35, 43, 43, 255);
	
	for (int x = 0; x < margin; x++)
	{
		DrawCircle(renderer, 5*board_size/4.0-x, 3*board_size/4.0+board_size/16.0-6*margin-x, margin/2.0);
		DrawCircle(renderer, 5*board_size/4.0-x, 3*board_size/4.0-board_size/16.0+3*margin/2.0+x, margin/2.0);
		DrawCircle(renderer, 5*board_size/4.0+x, 3*board_size/4.0-board_size/16.0+6*margin+x, margin/2);
		DrawCircle(renderer, 5*board_size/4.0+x, 3*board_size/4.0+board_size/16.0-3*margin/2.0-x, margin/2.0);
	}
	
	return;
}

void CreateWindow(NODE *node) {

	bool running = 1;
	int passed = 1;
	int changes;
	COORD cell;
	cell.row=0;
	cell.column=0;
	
	COORD temp_cell;

	// Inicializa o SDL;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Cria a janela;

	SDL_Window *window = SDL_CreateWindow("Othello-X", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 3*board_size/2, board_size, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Cria o renderizador acelerado e que acompanha a taxa
	// de atualização da tela;

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Inicializa o SDL mixer;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// Carrega a música;

	Mix_Music *backgroundSound = Mix_LoadMUS("kevin-macleod-8bit-dungeon-boss.mp3");
	Mix_Chunk *playEffect = Mix_LoadWAV("capture.wav");

	// Define uma variável do tipo evento;

	SDL_Event event;

	// Define uma variável do tipo evento do mouse;

	SDL_MouseButtonEvent *mouse = &event.button;

	// Roda a música de fundo;

	Mix_PlayMusic(backgroundSound, -1);

	// Enquanto estiver rodando, executa o código abaixo

	// Define um retângulo (quadrado);

	SDL_Rect rect;

	while (running)
	{

		// Define a cor de desenho do renderizador como sendo um
		// branco azulado (Alice Blue);

		SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255);

		// Limpa a janela com o branco azulado;

		SDL_RenderClear(renderer);
		
		ShowMenu(window, renderer, node);
		
		// Define a cor de desenho do renderizador como sendo um 
		// cinza esverdeado (Medium Jungle Green);

		SDL_SetRenderDrawColor(renderer, 35, 43, 43, 255);

		// Define o comprimento (board_size) e altura (board_size) do quadrado;

		rect.w = (board_size/SIZE)-margin/2.0;
		rect.h = (board_size/SIZE)-margin/2.0;

		// Desenha 32 quadrados para formar um tabuleiro na tela;

		for (int y = margin/4.0; y < board_size; y+=board_size/SIZE)
		{

			// Define a posição vertical do quadrado;

			rect.y = y;

			for (int x = margin/4.0; x < board_size; x+=board_size/SIZE)
			{
				// Define a posição horizontal do quadrado;

				rect.x = x;

				// Desenha o quadrado;

				SDL_RenderFillRect(renderer, &rect);
			}
		}

		// Desenha os Xzinhos e Bolinhas;
		
		for (int i = 0; i < SIZE; i++) 
		{
			for (int j = 0; j < SIZE; j++) 
			{	
				
				if ((*node).board[i][j] == CIRCLE)
				{
					// Define a cor do círculo como um ciano (Robin egg blue)
					// se o último lance não foi na casa em questão,
					// caso contrário, define como um lilás (Medium orchid)
					
					if (cell.row == i && cell.column == j)
					{
						SDL_SetRenderDrawColor(renderer, 196, 77, 255, 255);
					} else
					{
						SDL_SetRenderDrawColor(renderer, 0, 203, 204, 255);
					}
					DrawHollowCircle(renderer, (board_size/SIZE)*(i+1/2.0), (board_size/SIZE)*(j+1/2.0), (board_size/(2*SIZE)-2*margin));
				}
				if ((*node).board[i][j] == CROSS)
				{
					// Define a cor do X como um vermelho (Carmine red)
					// se o último lance não foi na casa em questão,
					// caso contrário, define como um lilás (Medium orchid)
					
					if (cell.row == i && cell.column == j)
					{
						SDL_SetRenderDrawColor(renderer, 196, 77, 255, 255);
					} else
					{
						SDL_SetRenderDrawColor(renderer, 255, 5, 52, 255);
					}
					DrawCross(renderer, (board_size/SIZE)*(i+1/2.0), (board_size/SIZE)*(j+1/2.0), (board_size/(2*SIZE)-margin));
				}
				if ((*node).board[i][j] == EMPTY) {
					temp_cell.row=i;
					temp_cell.column=j;
					changes=Simulate(node, temp_cell, 0);
					if (changes > 0) {
						passed = 0;
						if ((*node).player == CIRCLE) {
							// Define a cor do quadrado como um ciano (Robin egg blue)
							SDL_SetRenderDrawColor(renderer, 0, 203, 204, 255);
						} else {
							// Define a cor do quadrado como um vermelho (Carmine red)
							SDL_SetRenderDrawColor(renderer, 255, 5, 52, 255);
						}
						DrawSquare(renderer, (board_size/SIZE)*(i+1/2.0), (board_size/SIZE)*(j+1/2.0), (board_size/(3*SIZE)-margin));
					}
				}
			}
		}
		
		//if (passed == 2)
		//{
		//	running = 0;
		//}
		
		if (passed > 0) {
			(*node).player=((*node).player+1)%2;
			passed+=1;
		}

		//if ((*node).player == 1)
		//{
			if (passed == 0)
			{
				SDL_RenderPresent(renderer);
				//SDL_Delay(1000);	
				cell=SearchBest(*node, DEPTH);
				
				changes=Simulate(node, cell, 1);
				
				if (changes > 0)
				{
					// Se o lance for possível, toca um efeito sonoro;
					Mix_PlayChannel(-1, playEffect, 0);
				}
				passed=1;
			}
		//}

		// Mostra o que foi desenhado;

		SDL_RenderPresent(renderer);

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
				// efetuar um lance ou resetar o jogo;
				case SDL_MOUSEBUTTONDOWN:
				{
					if ((*mouse).button == SDL_BUTTON_LEFT)
					{
						// Transcreve as coordenadas da janela
						// para o tabuleiro;
						cell.row=(*mouse).x / (board_size/SIZE);
						cell.column=(*mouse).y / (board_size/SIZE);
						// Efetua um lance se for a vez do usuário;
						
						if ((*node).player == 0)
						{
							changes=Simulate(node, cell, 1);
							if (changes > 0) {
								// Se o lance for possível, toca um efeito sonoro;
								Mix_PlayChannel(-1, playEffect, 0);
							}
						}
						// Se as coordenadas do click satisfazem uma equação reduzida
						// da circunferência (com centro e raio coincidindo com o botão de restart),
						// então reinicia o jogo retomando ao menu;
						if (((*mouse).y-3*board_size/4.0)*((*mouse).y-3*board_size/4.0) + ((*mouse).x-5*board_size/4)*((*mouse).x-5*board_size/4) <= (board_size/16+margin)*(board_size/16+margin))
						{
							ResetBoard(node); // IMPORTANTE
						}
					}
					break;
				}
				// Se a janela for redimensionada, converte seu tamanho
				// mantendo a proporção e tentando manter seu lado como um múltiplo de (640/SIZE);
				case SDL_WINDOWEVENT:
				{
					if(event.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						board_size=event.window.data2 - event.window.data2 % (640/SIZE);
						// Define um tamanho mínimo para a janela;
						if (board_size < 320) {
							board_size=320;
						}
						SDL_SetWindowSize(window, 3*board_size/2, board_size);
						margin=SIZE*(board_size/640.0);
					}
				}
			}
		}
	}

	// Destrói o renderizador e a janela, e depois finaliza
	// o SDL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_FreeMusic(backgroundSound);
	Mix_FreeChunk(playEffect);
	Mix_CloseAudio();
	SDL_Quit();

	return;
}
