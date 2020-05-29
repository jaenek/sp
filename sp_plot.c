const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

typedef struct Plotter {
	bool interactive;
	Data *data;
} Plotter;

// TODO(#2): Add plot margins
// TODO(#4): Add resizing
void plot(Plotter *plotter)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_MAXIMIZED, &window, &renderer)) {
		printf("Couldn't create window and renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_SetRenderDrawColor(renderer, 34, 34, 34, 255);
	SDL_RenderClear(renderer);

	Data *data = plotter->data;

	float xmin = data->x[0], xmax = data->x[data->n-1];
	float ymin = data->y[0], ymax = data->y[0];
	for (size_t i = 1; i < data->n; i++) {
		float cur = data->y[i];
		if (ymin > cur)
			ymin = cur;
		if (ymax < cur)
			ymax = cur;
	}

	// TODO(#6): Axis scale becomes infinity if min and max are equal
	float xs = SCREEN_WIDTH/fabs(xmin-xmax);
	float ys = SCREEN_HEIGHT/fabs(ymin-ymax);

	printf("xmin: %f xmax: %f\nymin: %f ymax: %f\nxs: %f ys: %f\n",
		xmin, xmax, ymin, ymax, xs, ys);

	SDL_SetRenderDrawColor(renderer, 71, 147, 255, 255);
	for (size_t i = 0; i < data->n - 1; i++){
		int x1 = xs*(data->x[i]-xmin);
		int x2 = xs*(data->x[i+1]-xmin);
		int y1 = SCREEN_HEIGHT-ys*(data->y[i]-ymin);
		int y2 = SCREEN_HEIGHT-ys*(data->y[i+1]-ymin);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	SDL_RenderPresent(renderer);

	if (plotter->interactive) {
		SDL_Event e;
		while (SDL_WaitEvent(&e) != 0) {
			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
				break;
			}
		}
	}
}

