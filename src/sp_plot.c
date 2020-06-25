typedef struct Plotter {
	bool interactive;
	bool running;
	SDL_Renderer *renderer;
	SDL_Event e;
	FC_Font *font;
	Data *data;
} Plotter;

void drawplot(Plotter *plotter)
{
	SDL_SetRenderDrawColor(plotter->renderer, 255, 255, 255, 255);
	SDL_RenderClear(plotter->renderer);

	SDL_SetRenderDrawColor(plotter->renderer, 71, 147, 255, 255);
	for (size_t i = 0; i < plotter->data->visible-1; i++){
		SDL_RenderDrawLine(plotter->renderer,
				plotter->data->xscaled[i],   plotter->data->yscaled[i],
				plotter->data->xscaled[i+1], plotter->data->yscaled[i+1]);
	}
}

void drawcrosshair(SDL_Renderer *r, int x, int y) {
	SDL_Rect rect = {x-2, y-2, 5, 5};
	SDL_SetRenderDrawColor(r, 71, 147, 255, 255);
	SDL_RenderFillRect(r, &rect);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, x, 0, x, SCREEN_HEIGHT);
}

void input(void *arg) {
	Plotter *plotter = arg;

	SDL_RenderPresent(plotter->renderer);

	int index = 0;
	SDL_PollEvent(&plotter->e);
	switch (plotter->e.type) {
	case SDL_QUIT:
#if EMSCRIPTEN
		emscripten_cancel_main_loop();
#else
		plotter->running = false;
#endif
		break;
	case SDL_KEYDOWN:
		switch (plotter->e.key.keysym.sym) {
		case SDLK_ESCAPE:
#if EMSCRIPTEN
			emscripten_cancel_main_loop();
#else
			plotter->running = false;
#endif
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		drawplot(plotter);

		index = closestx(plotter->data, plotter->e.motion.x);

		drawcrosshair(plotter->renderer,
				plotter->data->xscaled[plotter->data->start+index],
				plotter->data->yscaled[plotter->data->start+index]);

		FC_Draw(plotter->font, plotter->renderer, 0, 0, "x: %f\ny: %f",
				plotter->data->x[plotter->data->start+index],
				plotter->data->y[plotter->data->start+index]);
		break;
	default:
		break;
	}
}

// TODO(#4): Add resizing
void plot(Plotter *plotter)
{
	SDL_Window *window = NULL;
	plotter->renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_MAXIMIZED, &window, &plotter->renderer)) {
		printf("Couldn't create window and renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	plotter->font = FC_CreateFont();
	FC_LoadFont(plotter->font, plotter->renderer,
			"font/FreeMono.otf",     20,
			FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);

	prepare(plotter->data);

	drawplot(plotter);

	if (plotter->interactive) {
#if EMSCRIPTEN
		emscripten_set_main_loop_arg(input, (void *)plotter, -1, 1);
#else
		plotter->running = true;
		while (plotter->running) {
			input(plotter);
		}
#endif
	}

	FC_FreeFont(plotter->font);
}
