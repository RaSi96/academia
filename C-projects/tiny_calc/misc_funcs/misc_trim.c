# include "../src/claytor.h"

char *trim(char *src_array)
{
	/* This function aims to remove all spaces (0x20) from a given input array
	 * using simple array indexing. If the source array element is not a space,
	 * the trimmed array is unchanged and maintains pace with the source array.
	 * If the source array element is a space, the index of the trimmed array
	 * where the next non-space character goes is decremented so that the trimmed
	 * array is now one element behind the source array. Once the source array
	 * has been fully parsed, the trimmed array is returned.
	 */
	char *trimmed = src_array;
	uint8_t src_counter = REF_INACTIVE;
	uint8_t dest_counter = REF_INACTIVE;
	uint8_t src_len = strlen(src_array);	//strlen is used as a more direct boundary instead of expecting a null terminator
	for (; src_counter < src_len; src_counter++, dest_counter++)
	{
		if (!isspace(src_array[src_counter]))	//while not a space, keep pace with the source array
		{
			trimmed[dest_counter] = src_array[src_counter];
		}
		else dest_counter--;	//otherwise trail the source array by the number of spaces so far
	}
	/* Once the array has been trimmed, it will be of a shorter length than the
	 * source array and hence it has to be null terminated again.
	 */
	trimmed[dest_counter] = 0;
	return trimmed;
}
