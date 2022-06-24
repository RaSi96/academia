# include "../src/set_practical.h"

void set_create(char *contents)
{
	/* This function is our set (linked list) pseudo-constructor: given a char
	 * array, it checks how many working sets currently exist and appropriately
	 * populates the next empty one using parse_array(). In most use cases it is
	 * rather unlikely that more than 4 sets will be requested, but in the event
	 * that they are, this function will simply report an error statement and
	 * return. This function might seem to have some redundancy within the while
	 * loop in main(), but it is necessary to have in case file I/O for creating
	 * sets is requested.
	 */
	uint8_t u_set_amount = u_how_many_sets();
	switch (u_set_amount)
	{
		case 0:
		{
			set_store_g.set_1 = parse_array(contents, set_store_g.set_1);
			printf("Set 1:\n");
			set_print(set_store_g.set_1);
			break;
		}
		case 1:
		{
			set_store_g.set_2 = parse_array(contents, set_store_g.set_2);
			printf("Set 2:\n");
			set_print(set_store_g.set_2);
			break;
		}
		case 2:
		{
			set_store_g.set_3 = parse_array(contents, set_store_g.set_3);
			printf("Set 3:\n");
			set_print(set_store_g.set_3);
			break;
		}
		case 3:
		{
			set_store_g.set_4 = parse_array(contents, set_store_g.set_4);
			printf("Set 4:\n");
			set_print(set_store_g.set_4);
			break;
		}
		default:
		{
			fprintf(stderr, "set_create(): Only 4 sets are supported.\n");
			break;
		}
	}
}
