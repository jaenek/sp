typedef struct Plotter {
	bool interactive;
	Data *data;
} Plotter;

void drawplot(SDL_Renderer *renderer, Data *data)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 71, 147, 255, 255);
	for (size_t i = 0; i < data->visible-1; i++){
		SDL_RenderDrawLine(renderer,
				data->xscaled[i],   data->yscaled[i],
				data->xscaled[i+1], data->yscaled[i+1]);
	}
}

void drawcrosshair(SDL_Renderer *r, int x, int y) {
	SDL_Rect rect = {x-2, y-2, 5, 5};
	SDL_SetRenderDrawColor(r, 71, 147, 255, 255);
	SDL_RenderFillRect(r, &rect);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, x, 0, x, SCREEN_HEIGHT);
}

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

	FC_Font* font = FC_CreateFont();
	FC_LoadFont(font, renderer,
			"font/FreeMono.otf",     20,
			FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);

	prepare(plotter->data);

	drawplot(renderer, plotter->data);

	if (plotter->interactive) {
		bool running = true;
		SDL_Event e;
		while (running) {
			SDL_RenderPresent(renderer);

			int index = 0;
			SDL_PollEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
						running = false;
						break;
				default:
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				drawplot(renderer, plotter->data);

				index = closestx(plotter->data, e.motion.x);

				drawcrosshair(renderer,
						plotter->data->xscaled[plotter->data->start+index],
						plotter->data->yscaled[plotter->data->start+index]);

				FC_Draw(font, renderer, 0, 0, "x: %f\ny: %f",
						plotter->data->x[plotter->data->start+index],
						plotter->data->y[plotter->data->start+index]);
				break;
			default:
				continue;
			}
		}
	}

	FC_FreeFont(font);
}
