# include "../src/set_practical.h"

void set_print(set_element_t *src_head)
{
	/* This function simply prints out the value of every element within a set
	 * and reports the total number of elements within it. It receives the head
	 * pointer of a set and traverses it entirely printing out the set->value
	 * while tracking the number of iterations required to reach the end. The
	 * number of iterations required is the number of elements in the set.
	 */
	set_element_t *printer = src_head;
	uint8_t u_elements = 0;	//number of elements should begin at 0
	while (printer != NULL)	//move through the set until it's empty
	{
		printf("%d - ", printer->u_value);	//print the current value to the stdout buffer (without flushing it with '\n')
		printer = printer->next;
		u_elements++;	//increment our number of elements counter with each iteration
	}
	printf("END\n%d elements in set.\n", u_elements);	//flush the stdout buffer and print the number of set elements
	putchar('\n');	//to ensure stdout is empty and to make the output text more readable
} //end void set_print()
