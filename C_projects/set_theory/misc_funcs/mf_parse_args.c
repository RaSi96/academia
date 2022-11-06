# include "../src/set_practical.h"

void parse_args(int argc, char **argv)
{
	/* This function parses any arguments passed to the program during launch.
	 * Expanding upon the idea introduced in defaults(), the only current flags
	 * supported are "-f" to indicate file I/O (read in a set's element values
	 * from a file rather than taking them in from user input), "-n" to request
	 * the program not to prompt for a restart after completing all set_ops, and
	 * "-h" to print the program's usage and help section. The most important
	 * idea to keep in mind when processing arguments to main() is that **argv
	 * (or *argv[]) is a 2D array: this means that the full text of the argument
	 * can be retrieved from argv's x-axis argv[argv_x] while the characters of
	 * each argument can be retrieved from argv's y-axis argv[argv_y]. Using
	 * this logic, the whole array of arguments (argv) is parsed: characters of
	 * argv are searched for within the accepted options string "fnh" and if a
	 * match is found, the option is processed accordingly; if no match is found
	 * then an error is reported and the program is reset to a "default" state.
	 */
	const char *const options = "fnh";	//file, no_restart, help
	uint8_t u_arg_err = 0;
	/* Remember argv is a 2D array, so we parse it using argv_x up to the number
	 * of CLAs submitted.
	 */
	for (int argv_x = 1; argv_x < argc; argv_x++)
	{
		/* We expected every argument to be separate (so "-fn file.txt" or "-fn
		 * aren't expected) and to be a maximum length of 2 characters (like "-f"
		 * or "-n".)
		 */
		for (int argv_y = 0; argv_y < 2; argv_y++)
		{
			if (argv[argv_x][argv_y] == '-')
			{
				continue;	//ignore unnecessary characters
			}
			else
			{
				char *search_result = strchr(options, argv[argv_x][argv_y]);	//check if the given CLA is valid
				if (search_result)	//if the given CLA is valid (exists within the options string), parse it
				{
					switch (argv[argv_x][argv_y])
					{
						case 0x66:	//"-f", file input
						{
							char readline[INPUT_SIZE] = {0};	//used by fread()
							/* Since we expect every CLA to be separate and only
							 * of 2 characters, we can also expect that every
							 * argument following "-f" will be the file to read
							 * out from. If this isn't the case, an error is
							 * printed and the file read process is cancelled.
							 */
							char *filename = argv[++argv_x];
							FILE *f_set = fopen(filename, "r");
							if (f_set == NULL)
							{
								fprintf(stderr, "parse_args(): Error accessing set source \"%s\".\n", argv[argv_x]);
								break;
							}
							/* fread() is used because if a file contains more
							 * than one line of CSVs to be parsed into a set, we
							 * need to be able to read the entire file as one
							 * set entirely rather than each line becoming its
							 * own set.
							 */
							fread(readline, sizeof(uint16_t), INPUT_SIZE, f_set);
							set_create(readline);	//create a set using the data read in from the set_file
							fclose(f_set);	//close the file descriptor once complete
							break;
						}
						case 0x6E:	//"-n", no restart
						{
							u_no_rr_flag_g = !u_no_rr_flag_g;
							break;
						}
						case 0x68:	//"-h", help
						{
							usage();
							break;
						}
						default:	//if for some reason the CLA can't be processed, increment the arg_err counter
						{
							u_arg_err++;
							break;
						}
					} //end switch (argv[argv_x][argv_y])
				} //end if (search_result)
				else	//otherwise if the given CLA is invalid, increment the arg_err counter
				{
					u_arg_err++;
				} //end else (if !search_result)
			} //end else (if argv[argv_x][argv_y] != '-')
		} //end for-loop parsing argv_y
	} //end for-loop parsing argv_x
	if (u_arg_err > 0)
	{
		printf("Unrecognised arguments, using default settings.\n");
		printf("Try \"-h\" for help.\n");
		defaults();	//if there were errors during argument processing, reset all flags to default
	}
} //end void parse_args()
