# ifndef CLAYTOR_H_
# define CLAYTOR_H_

/* DEFINITIONS AND INCLUSIONS */
# include <stdio.h>
# include <ctype.h>		//isdigit()
# include <string.h>	//strcspn()
# include <stdlib.h>	//exit()
# include <stdint.h>	//uints

# define INPUT_SIZE		128	//used by get_input() to limit the length of user input
# define NO_EXIT		100	//used to set the program's interactive loop
# define ALLOW_EXIT		99	//used to exit the program's interactive loop
# define BASE			10	//used by strtol() to parse decimal digits

# define LEFT_PAREN		6	//left parenthesis operator hierarchy '('
# define MULTIPLY		5	//multiplication operator hierarchy '*'
# define DIVISION		4	//division operator hierarchy '/'
# define ADDITION		3	//addition operator hierarchy '+'
# define SUBTRACT		2	//subraction operator hierarchy '-'

# define REF_ACTIVATE	1
# define RIGHT_PAREN	REF_ACTIVATE	//right parenthesis operator hierarchy ')'
# define OPERAND		REF_ACTIVATE	//used by type_flag to notify that the current stack item is an operand

# define REF_INACTIVE	0
# define OPERATOR		REF_INACTIVE	//used by type_flag to notify that the current stack item is an operator

# define STACK_EMPTY	(-1)	//used by pop() to indicate a stack is empty

/* STRUCTS */
typedef struct calc_stack
{
	/* Every element of our calculator stack is intended to hold a value, either
	 * an operand or an operator with the type_flag being set appropriately, a
	 * precedence value for operators (which affects calculation order) and a
	 * pointer to the next element on the stack (which reinforces calculation
	 * order.)
	 */
	enum {f_operator, f_operand} type_flag;
	int32_t value;
	int16_t precedence;
	struct calc_stack *next;
} calc_stack_t;

/* USERDEF FUNCTION PROTOTYPES */
//misc functions
calc_stack_t *parse_array(char *src_array);
char *get_input(char *dest_array, int n);
char *trim(char *src_array);

//math functions
int32_t op_add(int32_t augend, int32_t addend);
int32_t op_sub(int32_t minuend, int32_t subtrahend);
int32_t op_mul(int32_t multiplier, int32_t multiplicand);
int32_t op_div(int32_t dividend, int32_t divisor);
int32_t get_result(calc_stack_t *stack_head);
uint8_t u_isoperator(char test_var);

//stack functions
void stack_print(calc_stack_t *stack_head);
void stack_destroy(calc_stack_t *stack_head);
calc_stack_t *push(calc_stack_t *stack_head, int32_t value, uint8_t int_flag);
int32_t pop(calc_stack_t **stack_head);

# endif /* CLAYTOR_H_ */
