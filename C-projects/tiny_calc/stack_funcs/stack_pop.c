# include "../src/claytor.h"

int32_t pop(calc_stack_t **stack_head)
{
	/* This function simply retrieves the current value on the top of the stack
	 * and then destroys the element that contained it. The method of destroying
	 * an element on a stack is a little different from that of a linked list:
	 * unlike the latter, stacks require a double pointer to the stack's head to
	 * free items on the stack. This is because in order to free what the head
	 * currently points to after moving the head along to its next address, the
	 * head pointer has to be moved while the current address has to be freed.
	 * A single pointer would only move the head, which after free() would end
	 * up with the head pointing to garbage with the rest of the stack being
	 * lost.
	 */
	calc_stack_t *temp_head = *stack_head; //the temp pointer points to the address of stack's current head
	if (temp_head == NULL) //if it's null then return -1
	{
		return STACK_EMPTY;
	}
	int32_t result = temp_head->value; //otherwise get the value from the top of the stack
	*stack_head = temp_head->next; //assign the stack_head to point to what its own next pointer points to using the temp_head
	temp_head->next = NULL; //ensure that the temp_head now leads nowhere
	free(temp_head); //and then free the temp_head
	return result;
}
