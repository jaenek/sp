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

	int optind;
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
		case 'm': {
			if (argv[optind][2] == 'x') {
				if (sscanf(argv[++optind], "%d", &plotter->data->xmargin) != 1) {
					usage();
					exit(EXIT_FAILURE);
				}
			} else if (argv[optind][2] == 'y') {
				if (sscanf(argv[++optind], "%d", &plotter->data->ymargin) == 1) {
					plotter->data->ymargin *= 2;
				} else {
					usage();
					exit(EXIT_FAILURE);
				}
			} else {
				usage();
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 'l': {
			if (sscanf(argv[++optind], "%lu", &plotter->data->visible) != 1) {
				usage();
				exit(EXIT_FAILURE);
			}
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

	plot(plotter);

	free(plotter->data->x);
	free(plotter->data->y);
	free(plotter->data->xscaled);
	free(plotter->data->yscaled);
	free(plotter->data);
	free(plotter);
	exit(EXIT_SUCCESS);
}
