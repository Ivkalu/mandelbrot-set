#include <iostream>
#include <math.h>
#include <SDL.h>

#define MANDELBROT_WINDOW 500
#define JULIA_WINDOW 250
#define FPS 40
#define MAXITER 210
#define RED 2.5
#define GREEN 4.5
#define BLUE 7

static double zoomlvl = 0; //koliko je duboko zumirano
static SDL_Renderer* renderer;

long double duljina(long double x, long double y)
{
	return sqrt(x * x + y * y);
}

//za ra�unanje koliko iteracija treba dok ne eksplodira u beskona�nost
int iteration(long double xc, long double yc, long double xstari, long double ystari, long double x, long double y, int iteracija)
{
	double maxiter = MAXITER + zoomlvl / 5; //pove�avanje maksimalnog broja iteracija ovisno o dubini zumiranja
	if (iteracija > (maxiter))
		return 0; //ako je unutar mandelbrota, vra�a se 0 (crna boja)

	else if (x == xstari && y == ystari)
		return 0;

	else if (duljina(x, y) > 2)
		return iteracija;


	long double xnovi = x * x - y * y + xc;
	long double ynovi = 2 * x * y + yc;

	return iteration(xc, yc, x, y, xnovi, ynovi, iteracija + 1);
}



//za renderanje mandelbrot fraktala na slici lijevo
void mandelbrot(long double xcentar, long double ycentar, long double radijus)
{
	long double X; //koordinate u kompleksnoj ravnini
	long double Y;
	int broj; //broj iteracija

	for (int x = 0; x < MANDELBROT_WINDOW; x++) {
		for (int y = 0; y < MANDELBROT_WINDOW; y++) {

			X = ((long double)x / MANDELBROT_WINDOW) * 2 * radijus - radijus + xcentar;
			Y = ((long double)y / MANDELBROT_WINDOW) * 2 * radijus - radijus + ycentar;
			broj = iteration(X, Y, -2, -2, 0, 0, 0);

			if (broj == 0) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
			else {
				SDL_SetRenderDrawColor(renderer, RED * broj, GREEN * broj, BLUE * broj, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}

//za renderanje julia fraktala na slici desno
void julia(long double xc, long double yc, long double radijus)
{
	long double X; //koordinate u kompleksnoj ravnini
	long double Y;
	int broj; //broj iteracija

	for (int x = 0; x < JULIA_WINDOW; x++) {
		for (int y = 0; y < JULIA_WINDOW; y++) {

			X = ((long double)x / JULIA_WINDOW) * 2 * radijus - radijus;
			Y = ((long double)y / JULIA_WINDOW) * 2 * radijus - radijus;
			broj = iteration(xc, yc, -2, -2, X, Y, 0);

			if (broj == 0) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderDrawPoint(renderer, x + MANDELBROT_WINDOW, y);
			}
			else {
				SDL_SetRenderDrawColor(renderer, RED * broj, GREEN * broj, BLUE * broj, 255);
				SDL_RenderDrawPoint(renderer, x + MANDELBROT_WINDOW, y);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//stvaranje prozora
	SDL_Event event;
	SDL_Window* window = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MANDELBROT_WINDOW + JULIA_WINDOW, MANDELBROT_WINDOW, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	//framerate
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	int x; //koordinate mi�a na ekranu
	int y;
	int xprosli = 0; //prija�nje koordinate mi�a
	int yprosli = 0;

	long double xcentar = -0.75; //sredina ekrana u kompleksnoj ravnini
	long double ycentar = 0;
	long double radijus = 1.25; //polovica duljine ekrana u kompleksnoj ravnini

	long double X = 0; //koordinate mi�a u kompleksnoj ravnini
	long double Y = 0;

	bool pressed = false; //je li mi� pritisnut

	mandelbrot(xcentar, ycentar, radijus); //u�itavanje po�etnog mandelbrota

	while (true) {
		//framerate
		frameStart = SDL_GetTicks();
		
		//mouse position
		
		SDL_GetMouseState(&x, &y);
		//events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case (SDL_QUIT):
				SDL_DestroyRenderer(renderer);
				return 0;
				break;
			case (SDL_MOUSEWHEEL):
				zoomlvl += event.wheel.y;
				radijus = radijus - radijus * event.wheel.y / 40;
				break;
			case (SDL_MOUSEBUTTONDOWN):
				pressed = 1;
				//for correct position
				xprosli = x;
				yprosli = y;
				break;
			case (SDL_MOUSEBUTTONUP):
				pressed = 0;
				break;
			default:
				break;
			}
		}
		
		//on mouse movement
		if (pressed) {
			xcentar += ((long double)(xprosli - x)) / MANDELBROT_WINDOW * 2 * radijus;
			ycentar += ((long double)(yprosli - y)) / MANDELBROT_WINDOW * 2 * radijus;
		}

		xprosli = x; //pam�enje prija�njih koordinata mi�a
		yprosli = y;

		//X and Y for Julia set
		X = (long double)(x) / MANDELBROT_WINDOW * 2 * radijus - radijus + xcentar;
		Y = (long double)(y) / MANDELBROT_WINDOW * 2 * radijus - radijus + ycentar;

		//rendering
		SDL_RenderClear(renderer);
		mandelbrot(xcentar, ycentar, radijus); //tu se rendera mandelbrot
		if (not pressed) {
			julia(X, Y, 2); //tu se rendera julia
		}
		SDL_RenderPresent(renderer);

		//framerate
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	SDL_DestroyRenderer(renderer);
	return 0;
}