# include "../src/claytor.h"

void stack_print(calc_stack_t *stack_head)
{
	/* This function simply prints out details of an entire stack: the position
	 * or "index" of an element on it, whether the value of the element is that
	 * of an operator or an operand (and if it is an operator, it's symbol), the
	 * element precedence and its type_flag. This function is mainly used for
	 * debugging purposes, to analyse a calculator stack and its outputs.
	 */
	calc_stack_t *printer = stack_head;
	uint8_t u_index = REF_INACTIVE;	//used to track element position on the stack
	while (printer != NULL)
	{
		printf("[%d] ", u_index);
		if (printer->type_flag == f_operator)
		{
			printf("chr: [%c]\t", printer->value);
		}
		else
		{
			printf("val: %d\t", printer->value);
		}
		printf("pre: %d\tflg: %d\n", printer->precedence, printer->type_flag);
		printer = printer->next;
		u_index++;
	}
	putchar('\n');
}
