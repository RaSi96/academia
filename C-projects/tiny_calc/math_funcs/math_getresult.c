# include "../src/claytor.h"

int32_t get_result(calc_stack_t *stack_head)
{
	/* This function processes a calculator output stack to arrive at the final
	 * result of the expression requested by the user. It receives the head of a
	 * stack and then traverses it element by element, looking for a pattern of
	 * operator-operand-operand. If ooerator-operator is found, the old operator
	 * is pushed onto a secondary stack and the new operator is used. If instead
	 * operator-operand-operator is found, the old operator and single operand
	 * are pushed onto the secondary stack and the new operator is used. Only
	 * once the operator and both operand fields are valid will the final result
	 * of the expression be calculated, with the result too being pushed onto
	 * the secondary stack since it now replaces the operator and operands it
	 * consumed. The original output stack is then emptied onto the secondary
	 * stack (which should now be in POSTFIX) and the secondary stack is then
	 * emptied back into the output stack (reversing it into PREFIX.) Until the
	 * original output stack is null, the function will continue.
	 */
	enum {r_operand_1 = 1, r_operand_2 = 2} result_flags; //used to notify whether we have both operands or only one of them
	int32_t temp_value = REF_INACTIVE; //used as a helper variable when emptying one stack into another

	calc_stack_t *type_check = NULL; //pointer used to check the type of an output stack element (operand or operator)
	calc_stack_t *new_stack = NULL; //the secondary stack

	int32_t operand_1 = REF_INACTIVE;
	int32_t operand_2 = REF_INACTIVE;
	int32_t operator = REF_INACTIVE;
	int32_t result = REF_INACTIVE;

	while (stack_head != NULL)
	{
		type_check = stack_head;
		if (type_check->type_flag == OPERATOR)
		{
			/* If we have an operator, it needs to be saved regardless. However,
			 * if we already had an operator before this one (operator-operator)
			 * push the old one onto the secondary stack and save the new one.
			 * If we encountered operator-operand-operator, both of them are now
			 * invalid for a math op and are therefore pushed onto the secondary
			 * stack and the new operator is saved. Operand_1 is cleared and the
			 * flag reset.
			 */
			if (operator && result_flags == r_operand_1)
			{
				new_stack = push(new_stack, operator, OPERATOR);
				new_stack = push(new_stack, operand_1, OPERAND);
				operand_1 ^= operand_1;
				result_flags = 0;
			}
			else if (operator)
			{
				new_stack = push(new_stack, operator, OPERATOR);
			}
			operator = pop(&stack_head);	//the current operator has to be saved regardless
		}	//end if (u_isoperator(reader))
		else
		{
			/* If we have a digit instead of an operand, save it as operand_2 if
			 * operand_1 is already populated otherwise save it as operand_1 and
			 * appropriately flag which field has been populated.
			 */
			if (result_flags == r_operand_1)
			{
				operand_2 = pop(&stack_head);
				result_flags = r_operand_2;
			}
			else
			{
				operand_1 = pop(&stack_head);
				result_flags = r_operand_1;
			}
		}	//end else (if !u_isoperator(reader))
		/* We can only begin to calculate the result of an expression if we have
		 * an operator and 2 operands, though the check only looks for operand 2
		 * because we can't have operand 2 without operand 1, and we can't have
		 * operand 1 without an operator. Once this is the case, the result of
		 * the math operation is pushed onto the secondary stack, which should
		 * now be in POSTFIX notation with the result of the last op being the
		 * first value. In order to reverse this and have our result replace the
		 * operands it consumed, we simply repopulate the original output stack
		 * with all of the elements popped off the temporary stack. The function
		 * then proceeds as normal using the original output stack.
		 */
		if (result_flags == r_operand_2)
		{
			switch (operator)
			{
				case 0x2A: result = op_mul(operand_1, operand_2); break;	//'*'
				case 0x2F: result = op_div(operand_1, operand_2); break;	//'/'
				case 0x2B: result = op_add(operand_1, operand_2); break;	//'+'
				case 0x2D: result = op_sub(operand_1, operand_2); break;	//'-'
			}
			new_stack = push(new_stack, result, OPERAND);
			while (stack_head != NULL)	//empty the original stack into the temp
			{
				type_check = stack_head;
				uint8_t new_flag = type_check->type_flag;
				temp_value = pop(&stack_head);
				new_stack = push(new_stack, temp_value, new_flag);
			}
			while (new_stack != NULL)	//empty the temp stack into the original
			{
				type_check = new_stack;
				uint8_t new_flag = type_check->type_flag;
				temp_value = pop(&new_stack);
				stack_head = push(stack_head, temp_value, new_flag);
			}
			/* After our stacks have been properly sorted out (especially with
			 * the result of the last operation taking the place of the operator
			 * and pperands required to create it), cancel every variable out
			 * except for the result because that has to be returned eventually.
			 */
			operand_1 ^= operand_1;
			operand_2 ^= operand_2;
			operator ^= operator;
			result_flags = 0;
		}	//end if (operand_1 && operand_2)
	}	//end while (stack_head != NULL)
	stack_destroy(new_stack); //the secondary stack should have its heap destroyed before it exits function scope
	stack_destroy(stack_head); //the original stack should also have its head destroyed before it exits function scope
	return result;
}
