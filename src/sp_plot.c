typedef struct Plotter {
	bool interactive;
	Data *data;
} Plotter;

void render(SDL_Renderer *renderer, Data *data)
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
			"font/FreeSans.ttf",     20,
			FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);

	prepare(plotter->data);

	render(renderer, plotter->data);

	SDL_RenderPresent(renderer);

	if (plotter->interactive) {
		bool running = true;
		SDL_Event e;
		while (SDL_WaitEvent(&e) != 0 && running) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN)
				continue;

			render(renderer, plotter->data);

			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
					running = false;
					break;
			default:
					break;
			}

			int index = 0;
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
					index = closest(plotter->data, e.button.x, e.button.y);
					FC_Draw(font, renderer, 0, 0, "x: %f\ny: %f",
							plotter->data->x[plotter->data->start+index],
							plotter->data->y[plotter->data->start+index]);
					break;
			default:
					break;
			}

			SDL_RenderPresent(renderer);
		}
	}

	FC_FreeFont(font);
}
