typedef struct Data {
	FILE *input;
	char delim;
	size_t n;
	float *x;
	float *y;
} Data;

int main(int argc, char **argv);
void usage();
void readfromfile(Data *data, char *filename);
void readdata(Data *data);
