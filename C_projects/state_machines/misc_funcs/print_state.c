# include "../src/fsm_practical.h"

void print_state(aircraft *craft)
{
	/* This function prints out the current state of all mechanical facets that
	 * are being controlled by the landing gear control system.
	 */
	printf("------------------------------------\n");
	printf("\t**Aircraft state**\t\n");
	printf("------------------------------------\n");
	printf("Pilot lever:\t\t");
	switch (craft->pilot_lever)
	{
		case LEV_RISE:		printf("lever_rising\n");	break;
		case LEV_UP:		printf("lever_up\n");		break;
		case LEV_FALL:		printf("lever_falling\n");	break;
		default:			printf("lever_down\n");		break;
	}

	printf("Limit switch:\t\t");
	(craft->limit_switch == SW_OPEN)? printf("limit_open\n") : printf("limit_closed\n");

	printf("Squat switch:\t\t");
	(craft->squat_switch == SW_OPEN)? printf("squat_open\n") : printf("squat_closed\n");

	printf("Landing gear:\t\t");
	(craft->landing_gear == GEAR_DOWN)? printf("gear_down\n") : printf("gear_up\n");

	printf("Cockpit lights:\t\t");
	switch (craft->cockpit_lights)
	{
		case LIT_RED:		printf("lights_red\n");		break;
		case LIT_GREEN:		printf("lights_green\n");	break;
		default:			printf("lights_off\n");		break;
	}

	printf("Direction valve:\t");
	(craft->direction_valve == DVAL_DOWN)? printf("valve_down\n") : printf("valve_up\n");

	putchar('\n');
} //end void print_state()
