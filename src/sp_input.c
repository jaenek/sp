typedef struct Data {
	FILE *input;
	char delim;
	size_t n;
	size_t visible;
	size_t start;
	float *x;
	float *y;
	int *xscaled;
	int *yscaled;
	int xmargin;
	int ymargin;
} Data;

void readdata(Data *data)
{
	// TODO(#5): Add labels
	float x = 0.0f, y = 0.0f;
	char fmt[6];
	snprintf(fmt, 6, "%%f%c%%f", data->delim);
	while (fscanf(data->input, fmt, &x, &y) == 2) {
		data->n++;
		data->x = realloc(data->x, data->n*sizeof(float));
		data->y = realloc(data->y, data->n*sizeof(float));
		data->x[data->n-1] = x; data->y[data->n-1] = y;
	}
}

void readfromfile(Data *data, char *filename)
{
	data->input = fopen(filename, "r");
	if (data->input) {
		readdata(data);
	} else {
		fprintf(stderr, "Could not open %s: No such file or directory\n",
			filename);
		exit(EXIT_FAILURE);
	}
	fclose(data->input);
}

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

	float xscale = (SCREEN_WIDTH-data->xmargin)/fabs(xmin-xmax);
	float yscale = (SCREEN_HEIGHT-data->ymargin)/fabs(ymin-ymax);

	if (isinf(yscale)) {
		data->ymargin = SCREEN_HEIGHT;
		yscale = 1;
	}

	data->ymargin /= 2;

	data->xscaled = malloc(data->visible*sizeof(int));
	data->yscaled = malloc(data->visible*sizeof(int));
	for (size_t i = data->start, j = 0; i < data->n-1; i++, j++){
		data->xscaled[j]   = xscale*(data->x[i]-xmin)+data->xmargin;
		data->xscaled[j+1] = xscale*(data->x[i+1]-xmin)+data->xmargin;
		data->yscaled[j]   = SCREEN_HEIGHT-yscale*(data->y[i]-ymin)-data->ymargin;
		data->yscaled[j+1] = SCREEN_HEIGHT-yscale*(data->y[i+1]-ymin)-data->ymargin;
	}
}

int closest(Data *data, int x, int y) {
	int index = 0;
	float minlen = (data->xscaled[0]-x)*(data->xscaled[0]-x)+(data->yscaled[0]-y)*(data->yscaled[0]-y);
	for (size_t i = 1; i < data->visible-1; i++) {
		float len = (data->xscaled[i]-x)*(data->xscaled[i]-x)+(data->yscaled[i]-y)*(data->yscaled[i]-y);
		if (len < minlen) {
			index = i;
			minlen = len;
		}
	}

	return index;
}
