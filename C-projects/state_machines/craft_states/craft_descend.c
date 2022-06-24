# include "../src/fsm_practical.h"

uint8_t craft_descend(void)
{
	/* The function that sets up the aircraft descent state. By this point the
	 * controls for the landing gear are enabled and the pilots intend to land
	 * the aircraft.
	 */
	printf("Craft descending (dropping altitude)\n");
	A320.pilot_lever 		= LEV_FALL;
	A320.cockpit_lights		= LIT_RED;
	print_state(&A320);

	sleep(SNOOZE);
	current_state = STATE_LANDING;
	return REF_ACTIVATE;
} //end uint8_t craft_descend()
