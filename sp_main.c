void usage()
{
	// TODO(#3): Write proper usage function
	fprintf(stderr, "Usage: sp [-d] [file...]\n");
}

int main(int argc, char **argv)
{
	Plotter *plotter = calloc(1,sizeof(Plotter));
	plotter->interactive = true;
	plotter->data = calloc(1,sizeof(Data));
	plotter->data->delim = ';';

	size_t optind;
	for ( optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
		switch (argv[optind][1]) {
		case 'd': {
			if (argc - optind > 1) {
				plotter->data->delim = argv[++optind][0];
			} else {
				usage();
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 'n': {
			plotter->interactive = false;
			break;
		}
		case '\0': break; // do not exit on - (stdin)
		default:
			usage();
			exit(EXIT_FAILURE);
		}
	}

	if (argc > optind) {
		readfromfile(plotter->data, argv[optind]);
	} else {
		plotter->data->input = stdin;
		readdata(plotter->data);
	}

	for (size_t i  = 0; i < plotter->data->n; i++) {
		printf("x: %f, y: %f\n", plotter->data->x[i], plotter->data->y[i]);
	}

	plot(plotter);

	exit(EXIT_SUCCESS);
}
