# include "../src/claytor.h"

void stack_destroy(calc_stack_t *stack_head)
{
	/* This function, essentially a wrapper to pop(), destroys an entire stack.
	 * It receives the head of a stack to terminate, uses an interim value to
	 * receive the popped element' value, then cancels itself out. The actual
	 * repositioning and deallocation of stack elements is carried out by pop().
	 * This function simply calls pop until the head of the stack points to 0x0.
	 */
	int32_t interim_value = REF_INACTIVE;
	while (stack_head != NULL)
	{
		interim_value = pop(&stack_head);
		interim_value ^= interim_value;
	}
} //end void stack_destroy()
