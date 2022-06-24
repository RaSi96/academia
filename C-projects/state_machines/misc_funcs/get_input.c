# include "../src/fsm_practical.h"

char *get_input(char *dest_array, int input_size)
{
	/* This function exists as a wrapper to fgets(): setting the input buffer to
	 * stdin automatically and also parsing out the newline terminator that is
	 * read in by fgets after the stream from stdin has ended. If for whatever
	 * reason stdin is inaccessible, it will report an error and exit outright.
	 */
	setbuf(stdin, NULL);
	if (fgets(dest_array, input_size, stdin))
	{
		dest_array[strcspn(dest_array, "\n")] = 0;
		return dest_array;
	}
	else
	{
		fprintf(stderr, "get_input(): Error reading from stdin.\n");
		//exit(EXIT_FAILURE);
		return NULL;
	}
} //end char *get_input()
