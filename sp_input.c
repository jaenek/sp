typedef struct Data {
	FILE *input;
	char delim;
	size_t n;
	float *x;
	float *y;
	float xscale;
	float yscale;
	float xmargin;
	float ymargin;
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
