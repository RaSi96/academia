# include "../src/claytor.h"

calc_stack_t *parse_array(char *src_array)
{
	/* This function parses the values contained within a character array into
	 * integer values that will be operated upon by the calculator. The purpose
	 * of this function is to populate the operator and output stacks in Polish
	 * Notation (aka PREFIX notation) so as to preserve operator precedence in a
	 * more effective way than infix notation. It begins by first trimming the
	 * input array of any spaces: this helps tremendously when there are numbers
	 * with more than one digit. Then it checks if the last element of the array
	 * is a digit and if so, the entire number is collected and then pushed onto
	 * the output stack. If the last element is an operator, its precedence is
	 * retrieved and it is appropriately placed onto the operator stack. If the
	 * last element isn't an operator or a digit, an error counter is incremented
	 * and the array is pushed forward. At the end of the entire operation, at
	 * least one item should be on the output stack and the entire array to have
	 * been understood. If this isn't the case, incorrect input was received
	 * and the user is informed via an error message from main(). Array handling
	 * is carried out using simple array indexing.
	 */
	calc_stack_t *output_head = NULL; //locally initialise the output stack
	calc_stack_t *operator_head = NULL; //locally initialise the operator stack

	src_array = trim(src_array); //remove spaces from the input array, if any

	uint8_t flag = REF_INACTIVE; //the flag used to set a stack element's type_flag
	uint8_t len_limit = REF_INACTIVE; //used to stay within array bounds; incremented every time the src_array is manipulated
	uint8_t parse_error = REF_INACTIVE; //in case the user gave in some unrecognised value

	uint16_t array_len = strlen(src_array); //there is no way we get an array length of >65535...
	char *src_sentinel = (src_array + (array_len - 1)); //negative indexing is especially effective for Polish notation.

	while (len_limit != array_len)
	{
		if (isdigit(src_sentinel[-0]))
		{
			/* Check 1: if we have a digit, we currently do not know if it's a
			 * single digit (0 to 9) or if it's one digit out of a larger number
			 * (100 to 109 for example.) To ensure we address the entire context
			 * we simply move the array until we reach something that is not a
			 * digit, then move the array back by 1 so that we can strtol() the
			 * full number. For example, src_sentinel[-0] points to 9, which is
			 * actually part of a full expression (1+89). To store the full "89"
			 * we move the array back until we reach '+', then move it forward
			 * so that it points to "89" and then strtol() 89. Once this is done
			 * we just move the array back again by 1 to point to the next valid
			 * character (i.e. it now points to "+89".)
			 */
			flag = OPERAND; //if we have a digit, it is an operand.
			char *err_ptr = NULL;
			while (!u_isoperator(src_sentinel[-0]))
			{
				if (len_limit == array_len) break;
				--src_sentinel; //keep moving back until we reach a non-operand value
				len_limit++; //crucial that this is incremented here to avoid misalignment
			}
			src_sentinel++; //move forward by 1 after the while loop so that we strtol() the correct thing
			uint32_t value = strtol(src_sentinel, &err_ptr, BASE);
			output_head = push(output_head, value, flag);
			--src_sentinel; //then move back by 1 so that we align correctly with where we should be
			src_array = src_sentinel; //the source array also has to be reassigned so that src_sentinel isn't lost
		}
		else if (u_isoperator(src_sentinel[-0]))
		{
			/* Check 2: if we are parsing an operator, we need to ensure a few
			 * thigs:
			 * 1) If our operator stack is empty:
			 * 		1.1) push the operator onto the operator stack
			 * 2) If we have a right parenthesis:
			 * 		2.1) push it onto the operator_stack
			 * 3) If we have a left parenthesis:
			 * 		3.1) pop everything off the operator stack into the output
			 * 		stack until we have a right parenthesis
			 * 		3.2) pop the right parenthesis but discard it
			 * 4) If we have a +, -, *, or /:
			 * 		4.1) if our current operator has a lower precedence than the
			 * 		one currently at the top of the operator_stack:
			 * 			4.1.1) pop the higher precedence operators off and into
			 * 			the output stack
			 * 			4.1.2) push the current operator onto the operator stack
			 * 		4.2) if the top of the operator_stack is a ')':
			 * 			4.2.1) push the current operator onto the operator stack
			 * These operations are essential to maintaining operator precedence.
			 */
			flag = OPERATOR; //if we receive a precedence, we have an operator.
			uint8_t op_precedence = u_isoperator(src_sentinel[-0]);
			if ((operator_head == NULL) || (op_precedence == RIGHT_PAREN))
			{
				/* 2.1, 2.2, 2.2.1: if we have a right paren or our operator
				 * stack is unpopulated, push what we received onto the stack.
				 */
				operator_head = push(operator_head, src_sentinel[-0], flag);
				--src_sentinel;
				src_array = src_sentinel;
				len_limit++;
			}
			else if (op_precedence == LEFT_PAREN)
			{
				/* 2.3: if we have a left parenthesis, pop everything off the
				 * operator stack into the output stack until we have a right
				 * parenthesis, at which point we pop that too but discard it
				 * (matching parentheses cancel each other out.)
				 */
				while ((operator_head != NULL))
				{
					/* 2.3.1: Until we either empty the operator stack or reach
					 * a right parenthesis, keep popping elements off because a
					 * right paren means a priority expression has ended whereas
					 * a NULL stack head means there is a paren mismatch.
					 */
					if (operator_head->precedence == RIGHT_PAREN)
					{
						break;
					}
					int32_t popped = pop(&operator_head);
					output_head = push(output_head, popped, flag);
				}	//end while ((operator_head != NULL)
				if (operator_head == NULL)
				{
					/* If we empty our operator stack without finding a matching
					 * left/right parenthesis, report an error and cancel the
					 * whole operation.
					 */
					fprintf(stderr, "parse_array(): mismatching parentheses.\n");
					return NULL;
				}
				else
				{
					/* 2.3.2: If we successfully found a matching right paren,
					 * pop it off the operator stack too but destroy it.
					 */
					int32_t rparen_popped = pop(&operator_head);
					rparen_popped ^= rparen_popped;
					--src_sentinel;
					src_array = src_sentinel;
					len_limit++;
				}
			}	//end else if (op_precedence == ')')
			else
			{
				/* Check 2.4: If we have a basic math operator that is of lesser
				 * precedence than what is currently on the stack, then until we
				 * reach an item on the operator stack that is of lesser or the
				 * same priority or we exhaust our operator stack, continue to
				 * pop elements off and onto the output stack. For instance, if
				 * we currently have '-' while the operator stack is populated
				 * with (+, *, /, -, *)), move the higher priority operators on
				 * to the output stack until we reach either the second '-' or
				 * the right parenthesis.
				 */
				if (op_precedence < operator_head->precedence)
				{
					while (operator_head != NULL)
					{
						/* If during this operation we reach an operator of the
						 * same priority as what we currently have or the stack
						 * is empty, stop immediately.
						 */
						if ((operator_head->precedence == RIGHT_PAREN) ||
							(operator_head->precedence == op_precedence))
							{
								break;
							}
						int32_t popped = pop(&operator_head);
						output_head = push(output_head, popped, flag);
					}	//end while(operator_head != NULL)
				}	//end while (current operator precedence < stack precedence)
				/* Once the while loop is done and we either empty our operator
				 * stack or reach a right parenthesis, push what we currently
				 * have onto the operator stack.
				 */
				operator_head = push(operator_head, src_sentinel[-0], flag);
				--src_sentinel;
				src_array = src_sentinel;
				len_limit++;
			}	//end else (if we have a +, -, *, /)
		}	//end else if (u_isoperator(src_array[0]))
		else
		{
			/* Check 3: if we don't have an operator or a digit, move the source
			 * array forward by one item and increment an error counter.
			 */
			parse_error++;
			--src_sentinel;
			src_array = src_sentinel;
			len_limit++;
		}
	}	//end while (src_array[-0] != 0x0)
	if ((output_head == NULL) || (parse_error > 0))
	{
		/* At the end of everything we should expect our output stack to have at
		 * least one element and for everything from the src_array to have been
		 * successfully parsed. If this is not the case, return NULL.
		 */
		return NULL;
	}
	else
	{
		/* If our output stack is populated, then most likely so is our operator
		 * stack. If this is the case, empty the operator stack into the output
		 * stack so that we have only one final stack that preserves operator
		 * precedence without any parentheses.
		 */
		while (operator_head != NULL)
		{
			calc_stack_t *traverse_pointer = operator_head;
			flag = traverse_pointer->type_flag;
			int32_t op_popped = pop(&operator_head);
			output_head = push(output_head, op_popped, flag);
		}
	}
	stack_destroy(operator_head);	//in case our operator stack heap wasn't fully emptied for some reason, destroy it here
	return output_head;
}
