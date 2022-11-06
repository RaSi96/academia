# include "../src/claytor.h"

calc_stack_t *push(calc_stack_t *stack_head, int32_t value, uint8_t int_flag)
{
	/* This function is used to add or "push" elements onto a stack. It simply
	 * allocates a new stack node on the heap, assigns the node its requested
	 * flag and value and checks if the node is supposed to be an operator or an
	 * operand: operators receive precedence, operands do not. Finally, the new
	 * node is pointed to what the stack head currently points to and then the
	 * stack head is reassigned to point to the new node. Essentially the stack
	 * is front-loaded, which facilitates a LIFO mechanism.
	 */
	calc_stack_t *newnode = (calc_stack_t *)malloc(sizeof(calc_stack_t));
	if (newnode == NULL)
	{
		fprintf(stderr, "malloc() failure, exiting.\n");
		exit(EXIT_FAILURE);
	}
	newnode->type_flag = int_flag; //assign the new stack item its type
	newnode->value = value; //then assign it the item value which was given
	if (int_flag == 0)
	{
		newnode->precedence = u_isoperator(value); //ensure only actual operators receive a precedence
	}
	newnode->next = stack_head; //then position the new node on the top
	stack_head = newnode; //and point the head to it (providing LIFO behaviour)
	return stack_head;
}
