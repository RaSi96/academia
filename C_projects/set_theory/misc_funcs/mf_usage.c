# include "../src/set_practical.h"

void usage(void)
{
	printf("set_practical CLA usage:\n");
	printf("\"-f\" [filename.ext]: read set values from a file in a directory.\n");
	printf("\tIf unused, the program will launch interactively.\n");
	printf("\"-n\": do not prompt for restart, exit after set operations complete.\n");
	printf("\"-h\": print this help section.\n");
	putchar('\n');
	printf("[*] A minimum of 2 sets are required with a maximum of 4.\n");
	printf("[*] Sets can only be numeric. Alphanumeric sets are unsupported.\n");
		printf("[*] Floating point numbers aren't supported (whole numbers only.)\n");
	printf("[*] Try not to combine option switches, instead specify them separately.\n");
	printf("[*] If errors occurred during set creation the program will begin interactively.\n");
	exit(EXIT_SUCCESS);
} //end void usage()
