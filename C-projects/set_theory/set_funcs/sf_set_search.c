# include "../src/set_practical.h"

int set_search(set_element_t *src_head, uint16_t u_search_value)
{
	/* This function is used to report whether a particular value already exists
	 * within a set or not. It receives the head of the set to search through as
	 * a pointer as well as the value to look for as a uint16_t, then begins
	 * traversing through the list comparing the value of each element with the
	 * search_value. If a match is found, it will return the index within the set
	 * of the element that contains the matching value, otherwise it returns -1.
	 * Such functionality is useful for checking an existing set for duplicates.
	 */
	set_element_t *lantern = src_head;
	uint8_t u_location = 0;	//index of the duplicate value within the list
	while (lantern != NULL)	//traverse the entire list looking for search_value
	{
		if (lantern->u_value == u_search_value)
		{
			return u_location;	//if a match is found return its index
		}
		lantern = lantern->next;	//if there's no match with the current set->value then move on
		u_location++;	//increment our location counter
	}
	return NOT_FOUND;	//if no matchis found at all, return -1
} //end int set_search()
