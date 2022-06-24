# include "../src/set_practical.h"

void set_destroy(set_element_t *src_head)
{
	/* This function is used to destroy unused, unnecessary, or unneeded sets
	 * entirely. It receives a head of a set as a pointer and begins traversing
	 * the list element by element, keeping a track of where to go next in a
	 * temporary variable before freeing the element it is currently at. The
	 * responsibility of reassigning the set's head pointer to NULL lies with
	 * the calling function.
	 */
	set_element_t *destroyer = src_head;
	set_element_t *temp_next = NULL;	//this keeps a track of where to go next before free'ing the current node
	while (destroyer != NULL)	//traverse the entire list until its points to NULL
	{
		temp_next = destroyer->next;	//save the location of the next node to free before destroying it
		free(destroyer);
		destroyer = temp_next;	//move onto the next node
	}
} //end void set_destroy()
