# ifndef FSM_PRACTICAL_H_
# define FSM_PRACTICAL_H_

/* INCLUSIONS AND DEFINITIONS */
# include <stdio.h>
# include <string.h> //strcspn(), strncmp()
# include <unistd.h> //close(), read(), sleep()
# include <sys/epoll.h> //epoll() family of functions

# define WAIT			5000 //used by epoll_wait() to time the interrupt checks
# define INPUT_SIZE		6 //used to set the size of input buffers
# define LONG_SNOOZE	7 //used by sleep() to simulate flying

# define REF_ZERO		0x00
# define REF_INACTIVE	REF_ZERO //used as a function return value
# define SW_CLOSE		REF_ZERO //used by limit_switch and squat_switch
# define DVAL_DOWN		REF_ZERO //used by direction_valve
# define LEV_RISE		REF_ZERO //used by pilot_lever to simulate lever_rising
# define GEAR_DOWN		REF_ZERO //used by landing_gear to simulate gear_down
# define LIT_GREEN		REF_ZERO //used by cockpit_lights to set green colour
# define STATE_GROUND	REF_ZERO //first of the 6 aircraft states

# define REF_ONE		0x01
# define REF_ACTIVATE	REF_ONE //used as a function return value
# define SW_OPEN		REF_ONE //used by limit_switch and squat_switch
# define DVAL_UP		REF_ONE //used by direction_valve
# define LEV_UP			REF_ONE //used by pilot_lever to simulate lever_up
# define GEAR_UP		REF_ONE //used by landing_gear to simulate gear_change
# define LIT_RED		REF_ONE //used by cockpit_lights to set red colour
# define STATE_TAKEOFF	REF_ONE //second of the 6 aircraft states

# define REF_TWO		0x02
# define LEV_FALL		REF_TWO //used by pilot_lever to simulate lever_falling
# define LIT_OFF		REF_TWO //used by cockpit_lights to turn off the lights
# define STATE_ASCEND	REF_TWO //third of the 6 aircraft states

# define REF_THREE		0x03
# define LEV_DOWN		REF_THREE //used by pilot_lever to simulate lever_down
# define SHORT_SNOOZE	REF_THREE //used by sleep() for a shorter wait time
# define STATE_CRUISE	REF_THREE //fourth of the 6 aircraft states

# define REF_FOUR		0x04
# define STATE_DESCEND	REF_FOUR //fifth of the 6 aircraft states

# define REF_FIVE		0x05
# define SNOOZE			REF_FIVE //used by sleep() to simulate taxiing
# define STATE_LANDING	REF_FIVE //sixth and final of the 6 aircraft states

/* STRUCTS & ENUMS */
typedef struct
{
	/* The mechanical controls of our aircraft that are monitored by the landing
	 * gear control system. The pilot lever moves first followed by the squat
	 * switch, both of which together dictate whether the aircraft is taking off
	 * or not. Their decision is followed by the landing gear either retracting
	 * or extending as directed by the direction_valve, followed by the limit
	 * switch closing after retraction or opening if extended. The lights in the
	 * cockpit indicate whether the landing gear cluster is in transit or not. A
	 * bitfield is used to greatly reduce the amount of space required for the
	 * struct.
	 */
	uint8_t pilot_lever		: 2, //4 states, requires 2 bits
			limit_switch	: 1, //2 states, requires 1 bit
			squat_switch	: 1, //2 states, requires 1 bit
			landing_gear	: 1, //2 states, requires 1 bit
			cockpit_lights	: 2, //3 states, requires 2 bits
			direction_valve	: 1; //2 states, requires 1 bit
} aircraft;

/* GLOBAL VARIABLES */
//defined in fsm_practical.c [main()]
extern uint8_t current_state;
extern aircraft A320;

/* USERDEF FUNCTION PROTOTYPES */
//miscellaneous functions
void print_state(aircraft *craft);
char *get_input(char *dest_array, int input_size);

//craft_state functions
uint8_t craft_grounded(void);
uint8_t craft_takeoff(void);
uint8_t craft_ascend(void);
uint8_t craft_cruise(void);
uint8_t craft_descend(void);
uint8_t craft_landing(void);

# endif /* FSM_PRACTICAL_H_ */
