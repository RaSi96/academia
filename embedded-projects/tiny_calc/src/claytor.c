/* This is a basic calculator program written in C which uses PREFIX notation
 * to achieve results instead of chaining (which more common calculators use.)
 * This method also equips it with the awareness of operator precedence as well
 * as parentheses priority when evaluating an expression. Interestingly enough,
 * POSTFIX notation (aka Reverse Polish notation) is PREFIX notation but read
 * backwards (which is the same for POSTFIX to PREFIX.) INFIX notation is the
 * human-readable 5 + (6 * 7), which is much more computationally intensive.
 * A detailed explanation is included in the description of every function, but
 * an overview of the logic implemented is:
 * 1) retrieve an input expression from the user,
 * 2) trim the input string of any spaces and begin traversing it backwards,
 * 3) all numbers are stored on an output stack while operators are stored on an
 * operator stack (this preserves heirarchy),
 * 4) both stacks are merged into one output stack,
 * 5) the output stack is parsed using the pattern "operator-operand-operand",
 * which gives a mathematical result,
 * 6) the result is pushed onto the output stack at a position where it replaces
 * the operator and operands consumed to get it,
 * 7) the output stack is iterated through until it only contains a single item,
 * which is the final result.
 *
 * Note: a few caveats to the calculator in its current form: root extraction is
 * 		unsupported as of yet as is exponentiation; most importantly floats are
 * 		not supported. These caveats will be addressed in coming versions, even
 * 		set functionality that will use row-matrix mathematical operations.
 * 
 * Author: Rahul Singh
 * Date: 15 Jun 2022
 */

# include "claytor.h"

int main(void)
{
	char input[INPUT_SIZE] = {REF_INACTIVE};
	uint8_t exit_lock = NO_EXIT;
	printf(".:Welcome to Claytor:."); //calculator -> calc-lator -> claytor
	while (exit_lock != ALLOW_EXIT)
	{
		printf("\n> ");
		get_input(input, INPUT_SIZE);
		if (strncmp(input, "q", strlen("q")) == 0)
		{
			/* Only "q" will allow the program to exit, any other character will
			 * be ignored and instead passed as calculator input for processing.
			 */
			exit_lock = ALLOW_EXIT;
			continue;
		}
		calc_stack_t *parsed_input = parse_array(input);
		if (parsed_input == NULL)
		{
			/* If at all the user entered unintelligible characters that can not
			 * be recognised by the array parser, the parsed_input will be NULL.
			 * In this case, inform the user about it, suggest one method to
			 * exit, and restart. (Of course, Ctrl+C can also be used.)
			 */
			printf("Some errors parsing input. (Use \"q\" to quit.)\n");
		}
		else
		{
			/* By the time the result is retrieved, the output, operator and the
			 * secondary stack from get_result() should have been freed off the
			 * heap. Whenever the get_result or parse_array functions are called
			 * again, the pointers will be reinitialised to NULL. Note that the
			 * type_check function in get_result() is only a pointer to a stack,
			 * nothing is actually pushed onto it so there isn't a need to free
			 * it.
			 */
			int32_t result = get_result(parsed_input);
			printf("= %d\n", result);
		}
	}	//end while (exit_lock != ALLOW_EXIT)
	return 0;
}
