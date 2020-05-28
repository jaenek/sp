int main(int argc, char **argv)
{
	Data *data = calloc(1,sizeof(Data));
	data->delim = ';';

	// TODO(#1): Add interactive flag
	size_t optind;
	for ( optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
		switch (argv[optind][1]) {
		case 'd': {
			if (argc - optind > 1) {
				data->delim = argv[++optind][0];
			} else {
				usage();
				exit(EXIT_FAILURE);
			}
			break;
		}
		case '\0': break; // do not exit on - (stdin)
		default:
			usage();
			exit(EXIT_FAILURE);
		}
	}

	if (argc > optind) {
		readfromfile(data, argv[optind]);
	} else {
		data->input = stdin;
		readdata(data);
	}

	for (size_t i  = 0; i < data->n; i++) {
		printf("x: %f, y: %f\n", data->x[i], data->y[i]);
	}

	plot(data);

	exit(EXIT_SUCCESS);
}

void usage()
{
	// TODO: Write proper usage function
	fprintf(stderr, "Usage: sp [-d] [file...]\n");
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

void readdata(Data *data)
{
	// TODO: Add labels
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
