/* This is a C program showcasing the implementation and work of a Mealy finite
 * state machine. While Moore FSMs operate only on the basis of their current
 * state, Mealy state machines consider both their current state as well as any
 * inputs received, sometimes even reducing the number of states. State machines
 * and automata-based programming are useful conceptsbecause they offer superior
 * clarity and reduce program overhead. As an example, a program consisting of
 * multiple if/else/while loops can be much more efficient using a FSM because
 * repeated function and system calls can be eliminated.
 *
 * FSMs in C can be implemented in many ways: using structs, arrays, or switch
 * statements. All that is truly required is:
 * 1) time periods of automaton executions do not overlap (the machine can only
 * be in one state at a given time), and
 * 2) the only information passed from the previous step to the next is the
 * explicitly specified state.
 *
 * Using an array of function pointers addresses these requirements, where each
 * index of the array is one "state" that calls a function to prime the machine
 * for the next state. The main reason for choosing this approach is that arrays
 * and function pointers offer performance benefits along with a reduced size
 * overhead compared to structs and switch case statements: keeping in mind that
 * this kind of project targets an aircraft's landing gear embedded system, the
 * speed and size aspects were prioritised.
 *
 * The logic implemented here is:
 * 1) Initialise a simulated aircraft in its grounded state;
 * 2) Defer to the user to begin the simulation: once begun, set the next state
 * as the takeoff state and move ahead;
 * 3) When taking off, ensure that the craft's squat gear and pilot's lever are
 * both committed to the takeoff: if so, the next state is ascent, otherwise the
 * next state is the grounded state;
 * 4) After taking off, ensure that there isn't any emergency landing required:
 * if so, safely land the aircraft by setting it to the descent state, otherwise
 * set it to the cruise state;
 * 5) During the cruise state the aircraft's landing gear control system remains
 * disabled to conserve power. The software is still monitoring for pilot input
 * however to then set the aircraft into a desceding state. As implemented, the
 * simulation waits at craft_cruise for an infinite amount of time (a timeout of
 * -1) until the input to begin descending is received;
 * 6) After descending to an appropriate altitude, begin the landing state: if
 * the landing has to be aborted, the next state is takeoff with another input
 * check, otherwise the next state is the grounded state.
 *
 * Notes:
 * 1) a relatively large amount of #define macros are used to eliminate the need
 * for enum types. As mentioned, program size was a priority and to ensure that
 * it was addressed, since enums are implementation-defined but supposedly are
 * guaranteed to be the size of an int, a macro was used instead.
 *
 * 2) the aircraft is represented by a bitfield to again address the constraint
 * of program size. These choices also seemingly reduce program complexity.
 * 
 * 3) the only state in which user input is not polled for is only during the
 * descent state because the 2 states leading into descent (ascend and cruise)
 * require a very deliberate decision by the user/pilot to begin descending.
 *
 * 4) interestingly, the simulation allows the user to crash-land the aircraft
 * by interrupting descent and then interrupting takeoff. The reason descent
 * leads into takeoff is because during the descent phase it is possible for the
 * craft to be too low to abort descent, in which case the squat switch will
 * actually abort the attempt at taking off again and keep the gear extended.
 * 
 * Author: Rahul Singh
 * Date: 24 Jun 2022
 */

# include "fsm_practical.h"

/* GLOBAL VARIABLES */
uint8_t current_state = STATE_GROUND;
aircraft A320;

/* MAIN */
int main(void)
{
	/* The way this program is structured is using an array of function pointers
	 * to determine which state the aircraft is in based off user input. All six
	 * states correspond to one and only one function and the aircraft can be in
	 * only one of the six states at any given time. An infinite loop sets up
	 * the entire simulation with a check for whether the simulation is still
	 * active or not. An active simulation will continue to iterate infinitely.
	 * An inactive simulation will exit the program.
	 */
	uint8_t (*const state_array[6])(void) =
	{
		/* Setting up the array of 6 function pointers. Each function returns a
		 * uint8_t and take no arguments. Typedefs for the function pointers are
		 * not used because only the array uses the function pointers, there is
		 * no other access route within the program.
		 */
		&craft_grounded,
		&craft_takeoff,
		&craft_ascend,
		&craft_cruise,
		&craft_descend,
		&craft_landing
	};

	/* flight simulation */
	current_state = STATE_GROUND;
	uint8_t sim_return = REF_ACTIVATE;
	while (sim_return != REF_INACTIVE)
	{
		sim_return = state_array[current_state]();
		putchar('\n');
	}
	return 0;
} //end main()
