#include <SDL.h>
#include <stdio.h>

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

SDL_Renderer * renderer = NULL;
SDL_Texture * texture = NULL;


int init(int w, int h)
{
	//Initialization flag
	int success = 1;
    
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Game of life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = 0;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

void close()
{
	//Deallocate surface
    SDL_DestroyTexture(texture);
	SDL_FreeSurface( gXOut );
    SDL_DestroyRenderer(renderer);
	//Destroy window
	SDL_DestroyWindow( gWindow );

	//Quit SDL subsystems
	SDL_Quit();
}

int lolz(int w, int h)
{
    int i = 0;
    unsigned int current, last = 0;
	//Start up SDL and create window
	if(!init(w,h)) printf( "Failed to initialize!\n" );
	else {
        renderer = SDL_CreateRenderer(gWindow, -1, 0);
	    int quit = 0;
        char file_str[20];
	    //Event handler
	    SDL_Event e;
        gXOut = SDL_LoadBMP( "test\\0.bmp" );
        texture = SDL_CreateTextureFromSurface(renderer, gXOut);
		//While application is running
		while(!quit) {
			//Handle events on queue
			current = SDL_GetTicks();
            if (current > last + 50) {
                sprintf(file_str, "test\\%d.bmp", i);
                i++;
                SDL_FreeSurface( gXOut );
                gXOut = SDL_LoadBMP(file_str);
                texture = SDL_CreateTextureFromSurface(renderer, gXOut);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
                last = current;
            }
			while(SDL_PollEvent(&e) != 0 ) {
					//User requests quit
				if(e.type == SDL_QUIT) quit = 1;
		}
        }
	}
	//Free resources and close SDL
	close();

	return 0;
}