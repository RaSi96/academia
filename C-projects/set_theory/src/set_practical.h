# ifndef SET_PRACTICAL_H_
# define SET_PRACTICAL_H_

/* INCLUSIONS & DEFINITIONS */
# include <stdio.h>
# include <ctype.h>		//isdigit()
# include <stdint.h>	//uints
# include <stdlib.h>	//strtol(), malloc(), exit(), free()
# include <string.h>	//strcspn(), strlen(), strncmp(), strchr()
# include <stdarg.h>	//variadic functionality

# define BASE 10		//used by strtol() to parse decimal digits
# define INPUT_SIZE 128	//used to limit the length of user input
# define NOT_FOUND (-1)	//used by set_search() to imply no matching result
# define NO_EXIT 100	//used to set the program's interactive loop
# define ALLOW_EXIT 99	//used to exit the program's interactive loop

/* STRUCTS */
typedef struct set_struct
{
	/* This struct represents one element (node) of an entire set (linked list.)
	 * It contains its integer value and a next pointer directing the reader to
	 * the next element of the set.
	 */
	struct set_struct *next;
	uint16_t u_value;
} set_element_t;

typedef struct	set_heads
{
	/* This struct contains what will be the heads of every set (linked list.)
	 * We use a struct so that we can easily keep a track of which sets are in
	 * use and which ones are empty, rather than using multiple global variables
	 * for each individual head or local variables within main and then passing
	 * those pointers around to every function that requires awareness of which
	 * sets are active and which aren't. This way every function sees and has
	 * access to whichever sets are populated.
	 */
	set_element_t	*set_1,
					*set_2,
					*set_3,
					*set_4;
} set_head_store;

/* GLOBAL VARIABLES (initialised in set_practical.c) */
extern set_head_store set_store_g;	//the struct that contains pointers to working sets
extern uint8_t u_no_rr_flag_g;	//no restart: do not prompt for restart, exit after operations complete

/* USERDEF FUNCTION PROTOTYPES */
//misc functions
set_element_t *parse_array(char *src_array, set_element_t *dest_head);
char *get_input(char *dest_array, int n);
void parse_args(int argc, char **argv);
void usage(void);
void defaults();

//set functions
int set_search(set_element_t *src_head, uint16_t u_search_value);
void append_element(set_element_t **src_head, uint16_t u_value);
void set_destroy(set_element_t *src_head);
void set_print(set_element_t *src_head);
void set_create(char *contents);
uint8_t u_how_many_sets(void);

//set ops
void set_op_union(uint8_t u_sets, ...);
void set_op_inter(uint8_t u_sets, ...);

# endif /* SET_PRACTICAL_H_ */
