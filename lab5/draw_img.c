#include <SDL.h>
#include <stdio.h>

void update_img(SDL_Surface* surface, SDL_Renderer* renderer, SDL_Texture* texture, char* file_str)
{
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP(file_str);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int draw_img(int w, int h)
{
    int i = 0;
    unsigned int current, last = 0;
	int quit = 0;
    char file_str[20];

	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;	

	SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);

	SDL_Event e; 

	while(!quit) {
		current = SDL_GetTicks();
        if (current > last + 50) {
			last = current;
            sprintf(file_str, "test\\%d.bmp", i++);
			update_img(surface, renderer, texture, file_str);
        }
		while(SDL_PollEvent(&e) != 0) 
			if(e.type == SDL_QUIT) 
				quit = 1;
    }

    SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}