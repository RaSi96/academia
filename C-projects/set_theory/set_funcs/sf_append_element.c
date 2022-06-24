# include "../src/set_practical.h"

void append_element(set_element_t **src_head, uint16_t u_value)
{
	/* This function adds onto the end an element to an existing set with the
	 * specified value. It receives a pointer to the head of the set because if
	 * the set is unpopulated, it needs to be able to change what the head of
	 * the set points to; and the value that the new element should contain.
	 * After allocating the space for the new element on the heap, it traverses
	 * the set from the head until the end of it and then adjusts the previous
	 * element's next pointer to point to the new element created.
	 */
	set_element_t *new_elem = (set_element_t *)malloc(sizeof(set_element_t));
	if (new_elem == NULL)	//if for some reason malloc fails, report an error and exit
	{
		fprintf(stderr, "malloc() failure.\n");
		exit(EXIT_FAILURE);
	} //end if (new_elem == NULL)
	new_elem->u_value = u_value;	//assign the required value to the new element
	new_elem->next = NULL;	//appropriately position the new element

	if (*src_head == NULL)	//if the set was empty, assign the new element as the first element
	{
		*src_head = new_elem;
	}
	else	//otherwise traverse the set until an element points to NULL, then append the new element there
	{
		set_element_t *traverse = *src_head;
		while (traverse->next != NULL)	//crucial that this is "traverse->next" and not just "traverse"
		{
			traverse = traverse->next;
		}
		traverse->next = new_elem;	//a NULL next pointer means the end of the list, so append the new element there
	} //end else (if *src_head != NULL)
} //end void append_element()
