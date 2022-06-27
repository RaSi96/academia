# include "../src/claytor.h"

uint8_t u_isoperator(char test_var)
{
	/* This function is similar in effect to isdigit() or isspace(): if a given
	 * variable to be tested is either a plus, minus, asterisk or backslash
	 * (corresponding to the mathematical operators of addition, subtraction,
	 * multiplication or division) then return the value of the operator's
	 * precedence. Otherwise if the variable is instead a digit or something
	 * that isn't supported, return 0. The hierarchial values have been derived
	 * using PEMDAS.
	 */
	switch (test_var)
	{
		case 0x28: return LEFT_PAREN; //highest precedence
		case 0x2A: return MULTIPLY;
		case 0x2F: return DIVISION;
		case 0x2B: return ADDITION;
		case 0x2D: return SUBTRACT;
		case 0x29: return RIGHT_PAREN; //lowest precedence
		default: return REF_INACTIVE; //usually going to be used for digits
	}
}
