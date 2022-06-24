# include "../src/set_practical.h"

void set_op_union(uint8_t u_sets, ...)
{
	/* This function determines the union of how many ever sets are given to it,
	 * ignoring any duplicates whilst creating itself. It receives how many ever
	 * working sets exist, then uses the first one to populate its own union_set
	 * head that will later be referenced for duplicate detection. Once the union
	 * set exists, subsequent sets retrieved from va_arg() will have their values
	 * checked against the union set for duplication: only unique values will be
	 * appended as unions should only contain unique values. After all sets are
	 * parsed, the union set is destroyed. Unlike in set_op_inter(), here every
	 * pull to va_arg within the for-loop matches the number of valid arguments
	 * remaining (i.e. arg_loop doesn't have to be manually incremented anywhere.)
	 */
	set_element_t *union_set_head = NULL;
	uint8_t arg_loop = 0;
	va_list arg_pointer;

	va_start(arg_pointer, u_sets);	//we should parse as many sets as were given
	for (arg_loop = 0; arg_loop < u_sets; arg_loop++)
	{
		set_element_t *set_traverse = va_arg(arg_pointer, set_element_t *);
		if (union_set_head == NULL)	//if there isn't a union_set, populate it with the entirety of the first set
		{
			while (set_traverse != NULL)
			{
				append_element(&union_set_head, set_traverse->u_value);
				set_traverse = set_traverse->next;
			}
		} //end if (union_set_head == NULL)
		else	//otherwise if there is a union_set, check the value in every remaining set against it for duplicates
		{
			while (set_traverse != NULL)
			{
				int8_t dup_check = set_search(union_set_head, set_traverse->u_value);
				if (dup_check == NOT_FOUND)	//append only unique values to the union_set, otherwise move ahead
				{
					append_element(&union_set_head, set_traverse->u_value);
					set_traverse = set_traverse->next;
				}
				else set_traverse = set_traverse->next;
			} //end while (set_traverse != NULL)
		} //end if (union_set_head != NULL)
	} //end for-loop retrieving va_args
	printf("Parsed union set:\n");
	set_print(union_set_head);
	set_destroy(union_set_head);
	union_set_head = NULL;
	va_end(arg_pointer);
} //end set_element_t *set_op_union()
