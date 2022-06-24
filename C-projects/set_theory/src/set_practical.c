/* This is a C program that implements set data structure functionality using a
 * linked list. Here a set is assumed to be an unordered collection of positive
 * integers, and set operations such as union and intersection are performed on
 * as many sets as there are. Essentially, the program boils down to this logic:
 * 1) Begin by expecting a minimum of 2 working sets with a maximum of 4,
 * 2) Retrieve data input that each set will contain (either through file I/O or
 * interactively),
 * 3) Parse the input appropriately into an empty set,
 * 4) Run a union operation on how many ever sets exist,
 * 5) Run an intersection operation on how many ever sets exist,
 * 6) Destroy any existing sets (sets are dynamically allocated on the heap)
 * before returning from main
 * More detailed documentation on the mechanism of this program is included in
 * every function, and a few miscellaneous notes are appended to the bottom of
 * this file.
 *
 * Author: Rahul Singh
 * Date: 10 Jun 2022
 */

# include "set_practical.h"

/* GLOBAL VARIABLES */
set_head_store set_store_g = {NULL};
uint8_t u_no_rr_flag_g = 0;	//no restart: do not prompt for restart, exit after operations complete

/* MAIN */
int main(int argc, char **argv)
{
	/* Main is where the real driving of the program happens from. Here we start
	 * by checking the number of arguments passed to the program: if there are
	 * too little or too many, print the usage section and exit the program. If
	 * there are enough arguments passed, we then send argv for processing to
	 * parse_args(). After this is complete, we perform the next crucial check:
	 * how many working sets are there? We need to have a minimum of 2 working
	 * sets in order to perform any operations, so if for some reason file I/O
	 * failed or only one file was given, we prompt the user accordingly.
	 */
	char input[INPUT_SIZE] = {0};
	uint8_t u_confirm = 2;
	defaults();
	if ((argc < 1) || (argc > 10))
	{
		fprintf(stderr, "Invalid number of arguments passed.\n");
		usage();
	}
	else
	{
		parse_args(argc, argv);
	}

	uint8_t u_exit_lock = NO_EXIT;
	while(u_exit_lock != ALLOW_EXIT)
	{
		/* This is the section where we check to see how many working sets exist
		 * at runtime. As mentioned we need a minimum of 2, so if we don't have
		 * enough then we prompt the user accordingly. A crucial assumption here
		 * is that launching the program implies intent to use, so even if "-f"
		 * args can't be parsed, the program shouldn't exit but instead should
		 * prompt for user input. Additionally since we feature support for more
		 * than 2 sets, this is where we ask the user if they would like to use
		 * more than 2 sets. This part will always be executed if we have less
		 * than 2 working sets.
		 */
		if ((set_store_g.set_1 == NULL) || (set_store_g.set_2 == NULL))
		{
			if (set_store_g.set_1 == NULL)
			{
				fprintf(stderr, "Sets unpopulated, launching interactively...\n");
				printf("Enter set 1: ");
				get_input(input, INPUT_SIZE);
				set_create(input);
			}
			fprintf(stderr, "Set 2 unpopulated, minimum 2 required.\n");
			printf("Enter set 2: ");
			get_input(input, INPUT_SIZE);
			set_create(input);

			printf("Working with more than 2 sets? [y/n] ");
			get_input(input, INPUT_SIZE);
			u_confirm = strncmp(input, "y", strlen("y"));
			if (u_confirm == 0)
			{
				printf("Enter set 3: ");
				get_input(input, INPUT_SIZE);
				set_create(input);

				printf("{INFO: Max sets = 4, current = 3}\nOne more? [y/n] ");
				get_input(input, INPUT_SIZE);
				u_confirm = strncmp(input, "y", strlen("y"));
				if (u_confirm == 0)
				{
					printf("Enter set 4: ");
					get_input(input, INPUT_SIZE);
					set_create(input);
				}
			} //end if (the user wants more than 2 sets)
		} //end if set_1_head && set_2_head == NULL

		/* Here after ensuring we have at least 2 sets is where we send them for
		 * set operations (union, intersection.) If for some reason we still have
		 * less than 2 working sets, inform the user that something is wrong and
		 * print the usage section before exiting.
		 */
		uint8_t u_set_amount = u_how_many_sets();
		switch(u_set_amount)
		{
			case 2:
			{
				set_op_union(u_set_amount, set_store_g.set_1, set_store_g.set_2);
				set_op_inter(u_set_amount, set_store_g.set_1, set_store_g.set_2);
				break;
			}
			case 3:
			{
				set_op_union(u_set_amount, set_store_g.set_1, set_store_g.set_2,
										 set_store_g.set_3);
				set_op_inter(u_set_amount, set_store_g.set_1, set_store_g.set_2,
										 set_store_g.set_3);
				break;
			}
			case 4:
			{
				set_op_union(u_set_amount, set_store_g.set_1, set_store_g.set_2,
										 set_store_g.set_3, set_store_g.set_4);
				set_op_inter(u_set_amount, set_store_g.set_1, set_store_g.set_2,
										 set_store_g.set_3, set_store_g.set_4);
				break;
			}
			default:
			{
				fprintf(stderr, "main(): Error detecting populated sets.\n");
				usage();
			}
		} //end switch (set_amount) to determine how many working sets exist

		/* This section is where we simply check if the user should be prompted
		 * to restart the program or quit outright instead. This check will only
		 * succeed if the "-n" no_restart flag was passed to main; in all other
		 * situations the user will be prompted.
		 */
		if (u_no_rr_flag_g == 0)
		{
			printf("Restart? [y/n] ");
			get_input(input, sizeof(input));
			u_confirm = strncmp(input, "n", strlen("n"));
			if (u_confirm == 0)
			{
				u_exit_lock = ALLOW_EXIT;
			}
			else
			{
				set_destroy(set_store_g.set_1);
				set_store_g.set_1 = NULL;
				set_destroy(set_store_g.set_2);
				set_store_g.set_2 = NULL;
				set_destroy(set_store_g.set_3);
				set_store_g.set_3 = NULL;
				set_destroy(set_store_g.set_4);
				set_store_g.set_4 = NULL;
				putchar('\n');
			}
		} //end if (no_rr_flag_g == 0)
		else
		{
			u_exit_lock = ALLOW_EXIT;
		} //end else (if no_rr_flag_g != 0)
	} //end while (exit_lock != ALLOW_EXIT)

	/* Since all of the sets are allocated on the heap, even though most modern
	 * operating systems decisively recover all resources given to a program, it
	 * is a prudent practice to free any dynamically allocated memory before the
	 * program returns from main. Interestingly enough, free(NULL) is a no-op,
	 * and thus shouldn't have any side-effects if executed.
	 */
	set_destroy(set_store_g.set_1);
	set_store_g.set_1 = NULL;
	set_destroy(set_store_g.set_2);
	set_store_g.set_2 = NULL;
	set_destroy(set_store_g.set_3);
	set_store_g.set_3 = NULL;
	set_destroy(set_store_g.set_4);
	set_store_g.set_4 = NULL;
	return 0;
} //end int main()

/* Miscellaneous notes:
 * 1) A foundational concept used here is that one set is a linked list. Every
 * "set" will have a series of elements (nodes) connected to one another by a
 * head. The head is what will be used to traverse the set element by element.
 *
 * 2) the maximum size of an array element is 2^16 = 65536.
 *
 * 3) 128 characters are permitted per set array because it results in anywhere
 * between 18 (7 characters per element, ex: "65536, ") and 42 (3 characters per
 * element, ex: "1, ") set members.
 *
 * 4) Only positive values are accepted (even if we used int16_t, the first
 * isdigit() filter in parse_array() would neglect the leading "-".) This ties
 * into the naming convention followed within this source:
 * => prefix "u_" implies unsigned (uint8_t, uint16_t, ...)
 * => suffix "_g" implies global variable
 */
