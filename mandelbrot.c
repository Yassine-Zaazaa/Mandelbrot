#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 600
#define WIDTH 800
#define MAX_ITER 500

typedef struct s_complex{
	double x;
	double y;
}	t_complex;

void	SDL_ExitWithError()
{
	SDL_Log("Error: %s.\n", SDL_GetError());
	SDL_Quit();
	exit(-1);
}

int	main()
{
	SDL_Window	*win;
	SDL_Renderer	*render;
	SDL_bool	program_launched;
	SDL_Event 	event;
	SDL_Texture *currTexture;
    SDL_Texture *prevTexture;
	int			x;
	int			y;
	double			cursor_x;
	double			cursor_y;
	double		zoom;
	t_complex	center;

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitWithError();
	if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &render) != 0)
		SDL_ExitWithError();
	
	currTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	prevTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	cursor_x = 0.0;
	cursor_y = 0.0;
	zoom = 1.0;
	center = (t_complex){0, 0};
	program_launched = SDL_TRUE;

	while(program_launched)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					program_launched = SDL_FALSE;
					break;
				case SDL_MOUSEMOTION:
					cursor_x = event.motion.x;
					cursor_y = event.motion.y;
					break;
				case SDL_MOUSEWHEEL:
					if(event.wheel.y > 0)
							zoom *= 1.1;

					else if(event.wheel.y < 0)
							zoom /= 1.1;
					center.x +=(cursor_x / WIDTH - 0.5) / zoom;
    	    		center.y +=(cursor_y / HEIGHT - 0.5) / zoom;
					break;
			}
		}
		SDL_SetRenderTarget(render, currTexture);
		for(int x = 0; x < WIDTH; x++)
		{
			for(int y = 0; y < HEIGHT; y++)
			{
				t_complex c = {
					((double)x / WIDTH * 3.5 - 2.5) / zoom + center.x,
					((double)y / HEIGHT * 2 - 1) / zoom + center.y
				};
				t_complex z = {0,0};
				int iterations = 0;
				while(iterations < MAX_ITER)
				{
					if(z.x * z.x + z.y * z.y > 4)
						break;
					t_complex tmp = {
						z.x * z.x - z.y * z.y + c.x,
						2 * z.x *z.y + c.y
					};
					z = tmp;
					iterations++;
				}
				if(iterations == MAX_ITER)
					SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
				else
				{
					int color = iterations % 8 * 32;
					SDL_SetRenderDrawColor(render, color, color, color, 255);
				}
				SDL_RenderDrawPoint(render, x, y);
			}
		}
		SDL_SetRenderTarget(render, NULL);
		SDL_RenderCopy(render, prevTexture, NULL, NULL);
		SDL_RenderCopy(render, currTexture, NULL, NULL);
		SDL_RenderPresent(render);
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return (0);
}
