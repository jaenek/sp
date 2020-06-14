const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

typedef struct Plotter {
	bool interactive;
	Data *data;
} Plotter;

void prepare(Data *data)
{
	if (data->visible == 0 || data->visible > data->n)
		data->visible = data->n;
	else
		data->start = data->n - data->visible;

	float xmin = data->x[data->start], xmax = data->x[data->n-1];
	float ymin = data->y[data->start], ymax = data->y[data->start];
	for (size_t i = data->start+1; i < data->n; i++) {
		float cur = data->y[i];
		if (ymin > cur)
			ymin = cur;
		if (ymax < cur)
			ymax = cur;
	}

	data->xmin = xmin;
	data->ymin = ymin;

	data->xscale = (SCREEN_WIDTH-data->xmargin)/fabs(xmin-xmax);
	data->yscale = (SCREEN_HEIGHT-data->ymargin)/fabs(ymin-ymax);

	if (isinf(data->yscale)) {
		data->ymargin = SCREEN_HEIGHT;
		data->yscale = 1;
	}

	data->ymargin /= 2;
}


void render(SDL_Renderer *renderer, Data *data)
{
	SDL_SetRenderDrawColor(renderer, 71, 147, 255, 255);
	for (size_t i = data->start; i < data->n - 1; i++){
		int x1 = data->xscale*(data->x[i]-data->xmin)+data->xmargin;
		int x2 = data->xscale*(data->x[i+1]-data->xmin)+data->xmargin;
		int y1 = SCREEN_HEIGHT-data->yscale*(data->y[i]-data->ymin)-data->ymargin;
		int y2 = SCREEN_HEIGHT-data->yscale*(data->y[i+1]-data->ymin)-data->ymargin;
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	SDL_RenderPresent(renderer);
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
	FC_LoadFont(font, renderer, "font/FreeSans.ttf", 20, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);

	SDL_SetRenderDrawColor(renderer, 34, 34, 34, 255);
	SDL_RenderClear(renderer);

	prepare(plotter->data);

	FC_Draw(font, renderer, 0, 0, "TEST Data length: %d\n", plotter->data->n);
	render(renderer, plotter->data);


	if (plotter->interactive) {
		SDL_Event e;
		while (SDL_WaitEvent(&e) != 0) {
			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
				break;
			}
		}
	}

	FC_FreeFont(font);
}