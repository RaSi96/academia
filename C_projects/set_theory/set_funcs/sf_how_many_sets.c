# include "../src/set_practical.h"

uint8_t u_how_many_sets(void)
{
	/* This function is used to report how many working sets exist. It only looks
	 * at the heads of a set to determine whether a set exists because ideally
	 * if a set exists, it's head pointer should reference the first element that
	 * should have been allocated on the heap; i.e. it should not be NULL. Our
	 * set_destroy() function also sets set_heads to NULL after freeing every
	 * element within it, making the set_head the point to decide at.
	 */
	if (set_store_g.set_1 == NULL) return 0;	//no working sets
	else if (set_store_g.set_2 == NULL) return 1;	//1 working set
	else if (set_store_g.set_3 == NULL) return 2;	//2 working sets
	else if (set_store_g.set_4 == NULL) return 3;	//3 working sets
	else return 4;	//we can only have a maximum of 4 working sets
} //end uint8_t u_how_many_sets()
