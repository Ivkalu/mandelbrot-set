#include <iostream>
#include <math.h>
#include "SDL.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define MAXITER 255

#define RED 23
#define GREEN 3
#define BLUE 28


float duljina(float x, float y)
{
	return sqrt(x * x + y * y);
}



int iteration(float xc, float yc, float xstari, float ystari, float x, float y, int iteracija)
{


	if (iteracija > MAXITER) {
		return 0;
	}
	else if (x == xstari && y == ystari) {

		return 0;
	}
	else if (duljina(x, y) > 2) {
		return iteracija;
	}

	float xnovi = x * x - y * y + xc;
	float ynovi = 2 * x * y + yc;

	return iteration(xc, yc, x, y, xnovi, ynovi, iteracija + 1);

}

bool dubina(SDL_Renderer** renderer, SDL_Event* event, int iteracija)
{

	for (int i = 0; i < pow(2, iteracija); i++) {
		for (int j = 0; j < pow(2, iteracija); j++) {
			int X = WINDOW_WIDTH / pow(2, iteracija + 1) + i * WINDOW_WIDTH / pow(2, iteracija);
			int Y = WINDOW_HEIGHT / pow(2, iteracija + 1) + j * WINDOW_HEIGHT / pow(2, iteracija);

			float x = -2 + pow(2, -iteracija + 1) + i * pow(2, -iteracija + 2);
			float y = -2 + pow(2, -iteracija + 1) + j * pow(2, -iteracija + 2);

			int broj = iteration(x, y, -2, -2, 0, 0, -1);

			if (broj == 0) {

				SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
				SDL_RenderDrawPoint(*renderer, X, Y);
			}

			else {
				//std::cout << broj << std::endl;
				SDL_SetRenderDrawColor(*renderer, RED*broj, GREEN*broj, BLUE*broj, 255);
				SDL_RenderDrawPoint(*renderer, X, Y);
			}

			if (SDL_PollEvent(event) && (*event).type == SDL_QUIT)
				return 1;
		}
		SDL_RenderPresent(*renderer);
	}
	
	return 0;

}



int main(int argc, char* argv[])
{
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Julia Set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	int max = 200;
	for (int i = 0; i < max; i++) {

		if (dubina(&renderer, &event, i)) {

			break;
		}
	}
	while (1) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}
	SDL_DestroyRenderer(renderer);
	return 0;
}