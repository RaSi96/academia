# include "../src/claytor.h"

int32_t op_sub(int32_t subtrahend, int32_t minuend)
{
	/* Subtraction function used by the calculator. It subtracts operand_2 (the
	 * minuend) from operand_1 (the subtrahend.)
	 */
	int32_t diff = subtrahend - minuend;
	return diff;
} //end int16_t subtraction()
