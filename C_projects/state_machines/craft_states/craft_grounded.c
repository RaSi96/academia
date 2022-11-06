# include "../src/fsm_practical.h"

uint8_t craft_grounded(void)
{
	/* The function that sets our aircraft in a grounded state (wheels are on
	 * tarmac.) When the aircraft is grounded the hydraulic system of the craft
	 * monitors the pressure exerted on landing gear clusters of the aircraft
	 * and appropriately counterbalances it by applying pressure in the opposite
	 * direction. Once the aircraft is prepped for takeoff there is also a fail-
	 * safe system in case of pilot lever failure: gas-pressurised springs are
	 * primed to deploy the landing gear if the pilot's lever is inoperative.
	 */
	printf("Craft grounded.\n");
	printf("Hydraulic counter-pressure system engaged.\n");
	A320.pilot_lever		= LEV_DOWN;
	A320.limit_switch		= SW_OPEN;
	A320.squat_switch		= SW_CLOSE;
	A320.landing_gear		= GEAR_DOWN;
	A320.cockpit_lights		= LIT_GREEN;
	A320.direction_valve	= DVAL_DOWN;
	print_state(&A320);

	printf("Commence simulation? [y/n] ");
	char confirm[INPUT_SIZE] = {0};
	get_input(confirm, INPUT_SIZE);
	if (!strncmp(confirm, "y", strlen("y")))
	{
		/* Enable the pilot lever failsafe system and set the next target state
		 * of the aircraft to takeoff.
		 */
		printf("Landing gear emergency pressure springs engaged.");
		current_state = STATE_TAKEOFF;
		return REF_ACTIVATE;
	}
	else return REF_INACTIVE;
} //end uint8_t craft_grounded()
