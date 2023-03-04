# include "../src/set_practical.h"

set_element_t *parse_array(char *src_array, set_element_t *dest_head)
{
	/* This function parses the values contained within a character array into
	 * integer values that set elements will contain, as well as checking the
	 * set it's working on for duplicate values before appending values onto it.
	 * It receives a character array as well as the head of the set that should
	 * be appended to, then traverses the array using strtol() skipping spaces,
	 * commas and duplicate values. Because using strlen(array) after declaration
	 * will count spaces and commas (which doesn't happen during initialisation),
	 * we need to  manually increment the traversal iterator every time one of
	 * them is reached. After source array is empty, a pointer to the set is
	 * returned.
	 */
	if (*src_array == 0x0)	//check if the user entered a NULL string (accidentally pressed enter before anything else)
	{
		/* This tiny section is a sanity check: if there was no input received
		 * for any set prompt, the src_array will be empty. If this is the case,
		 * just replace the emptiness with a 0 because NULL sets cause problems
		 * for the intersection operation.
		 */
		src_array[0] = '0';
	}
	char *err_ptr = src_array;	//initialise an error_pointer to point to the source array (used by strtol())
	uint8_t u_dup_values = 0;	//number of duplicates found counter: incremented every time a duplicate is found
	int8_t dup_check = 0;	//duplication search result flag: if found, will be > 0 (receives "location" from set_search())
	while (err_ptr[0] != 0x0)	//because strtol() modifies its base array, we wait until the first character is NULL (0x00)
	{
		if (!isdigit(err_ptr[0]))	//if the first element of the src_array is not a valid digit, move ahead
		{
			++err_ptr;	//increment our error_pointer up to the next item in the source array
			src_array = err_ptr;	//then set our source array to what err_ptr points to
		} //end if (!isdigit(err_ptr[0]))
		else	//otherwise if the first element of the src_array is a valid digit, parse it with strtol()
		{
			uint32_t u_set_value = strtol(src_array, &err_ptr, BASE);
			dup_check = set_search(dest_head, u_set_value);	//check the existing set for duplicates
			if (dup_check == NOT_FOUND)	//if the value is unique, append it and move ahead
			{
				append_element(&dest_head, u_set_value);
				if (*err_ptr != 0x0)
				{
					/* This tiny snippet adds onto the sanity check: if there is
					 * only 1 valid digit in a set and it's parsed out, err_ptr
					 * will then point to nothing. If this is not the case (if
					 * *err_ptr != 0x0), then move err_ptr up by one and point
					 * the src_array to it to move that up by 1 as well. Other-
					 * wise don't manipulate err_ptr at all, simply return.
					 */
					++err_ptr;
					src_array = err_ptr;
				}
			}
			else	//otherwise if the value is a duplicate, increment the duplicate counter and move ahead
			{
				u_dup_values++;
				++err_ptr;
				src_array = err_ptr;
			}
		} //end else (if isdigit(err_ptr[0]) != 0)
	} //end for-loop that parses every element of the *src_array
	if (u_dup_values > 0)	//if we had any duplicate values within the source_array, report them
	{
		printf("%d duplicate values skipped.\n", u_dup_values);
	}
	return dest_head;
} //end set_element_t *parse_array()
