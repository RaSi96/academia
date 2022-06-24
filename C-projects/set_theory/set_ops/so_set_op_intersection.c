# include "../src/set_practical.h"

void set_op_inter(uint8_t u_sets, ...)
{
	/* This function determines the intersection of how many ever sets are given
	 * to it, ignoring all unique values whilst creating itself and recognising
	 * duplicates. The desired logic is that two sets form an intersection_set
	 * after which each subsequent set is checked for duplicate values within
	 * the existing intersection set, not with the previous 2 sets or the next
	 * one. This is because intersections are commutative, so the intersection
	 * of the first 2 sets and the intersection of the 3rd with the result from
	 * the first 2 will be equivalent to the intersection of sets 1, 2, and 3
	 * compared independently. Given this property, there are two stages to the
	 * way this function operates:
	 * 
	 * 1) We begin by pulling 2 sets from va_arg because we can be certain that
	 * va_arg will contain at least 2 valid arguments of type (set_element_t *).
	 * The intersection of these 2 sets will populate the intersection_set_head,
	 * regardless of whether the resulting intersection_set_head is NULL or not.
	 * 
	 * 2) We then loop through the number of sets contained in va_arg one at a
	 * time using the second comparator set as the existing intersection_set_head
	 * and duplicates here result in a temporary intersection set.
	 * 
	 * If a temporary intersection set exists, the original intersection set is
	 * destroyed and recreated with every element from the temporary set. If a
	 * temporary intersection set wasn't created (if there are no duplicates
	 * between the 3rd set and the result of the first 2), the intersection set
	 * is destroyed, set to NULL, and the function terminates immediately. This
	 * roundabout-ness is necessary to ensure that only duplicates from ALL of
	 * the sets passed into this function exist within the intersection set, not
	 * just duplicates between a pair of sets. After all sets are parsed, the
	 * intersection set is destroyed.
	 */
	set_element_t *intersection_set_head = NULL;
	set_element_t *temp_intersection = NULL;
	set_element_t *secondary_set = NULL;
	int8_t dup_check = 0;
	uint8_t u_arg_loop = 0;
	va_list arg_pointer;

	va_start(arg_pointer, u_sets);
	for (u_arg_loop = 0; u_arg_loop < u_sets; u_arg_loop++)
	{
		/* Every main_set pull from va_arg matches the for-loop increment counter,
		 * so there isn't a need to manually update arg_loop here.
		 */
		set_element_t *main_set = va_arg(arg_pointer, set_element_t *);
		if (u_arg_loop < 2)	//check if we already used 2 sets (if true, certainly an intersection set exists)
		{
			/* Every secondary_set pull from va_arg does not match the for-loop
			 * counter, hence arg_loop has to be manually updated here.
			 */
			secondary_set = va_arg(arg_pointer, set_element_t *);
			u_arg_loop++;
			while (main_set != NULL)	//traverse the main_set until the end checking each value for duplication
			{
				dup_check = set_search(secondary_set, main_set->u_value);
				if (dup_check == NOT_FOUND)	//if a duplicate isn't found, move onto the next iteration
				{
					main_set = main_set->next;
				}
				else	//otherwise append the duplicate value to the intersection set
				{
					append_element(&intersection_set_head, main_set->u_value);
					main_set = main_set->next;
				}
			} //end while (main_set != NULL)
		} //end if (arg_loop < 2)
		else	//if we have pulled 2 sets from va_arg, an intersection set exists so use that as the secondary_set
		{
			secondary_set = intersection_set_head;
			while (main_set != NULL)
			{
				dup_check = set_search(secondary_set, main_set->u_value);
				if (dup_check == NOT_FOUND)	//if a duplicate isn't found, move onto the next iteration
				{
					main_set = main_set->next;
				}
				else	//otherwise append the duplicate to a temporary intersection set and move on
				{
					append_element(&temp_intersection, main_set->u_value);
					main_set = main_set->next;
				}
			} //end while (main_set != NULL)
			if (temp_intersection == NULL)
			{
				/* if no duplicates exist between the the result of the first 2
				 * sets and the next one, there should be no intersection set at
				 * all.
				 */
				set_destroy(intersection_set_head);
				intersection_set_head = NULL;
				break;
			}
			else
			{
				/* Otherwise if duplicates exist between the result of the first
				 * 2 sets and the next one, empty the existing intersection set
				 * and begin copying the values from temp_intersection to the
				 * the original intersection set. This is because the first 2
				 * sets might have had 3 duplicate values, but between the 3rd
				 * set and the result of the first 2 sets we might have only 1.
				 */
				set_destroy(intersection_set_head);
				intersection_set_head = NULL;
				while (temp_intersection != NULL)	//traverse the whole temporary set appending every value to intersection_set
				{
					append_element(&intersection_set_head, temp_intersection->u_value);
					temp_intersection = temp_intersection->next;
				}
				set_destroy(temp_intersection);	//after completion, destroy the temporary set and restart
				temp_intersection = NULL;
			} //end else (if temp_intersection != NULL)
		} //end else (if intersection_set_head != NULL)
	} //end for-loop retrieving va_args
	printf("Parsed intersection set:\n");	//finally, print the remaining intersection set that should have been correctly parsed
	set_print(intersection_set_head);
	set_destroy(intersection_set_head);	//and then destroy it before returning control to main
	intersection_set_head = NULL;
	va_end(arg_pointer);
} //end set_element_t *set_op_inter()
