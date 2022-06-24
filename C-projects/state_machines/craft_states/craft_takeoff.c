# include "../src/fsm_practical.h"

uint8_t craft_takeoff(void)
{
	/* This is one of three nexus functions that poll for user input to change
	 * the state of the aircraft. During takeoff, 2 scenarios can occur: either
	 * the pilots really do intend to take off or the aircraft has bumped over
	 * something on the tarmac during relatively slow-speed movement. This is
	 * achieved by the hydraulic system monitoring the rising or falling edge of
	 * the pilot's lever as well as input from the landing gear's squat switch.
	 * To ensure that one situation prevails over the other, the squat switch
	 * and pilot's lever are monitored for 5 seconds to ensure that takeoff is
	 * assured before committing to retracting the landing gear (practically,
	 * signals may also be received and combined  from the craft's speedometer
	 * and altimeter.) If the landing is aborted within the 5 seconds either by
	 * pilot lever input or the squat switch retriggering, it doesn't retract
	 * the landing gear. For simulation purposes, epoll() is used as the trigger
	 * detector for the squat switch.
	 */
	printf("Detected wheels off the ground...\n");
	A320.pilot_lever		= LEV_RISE;
	A320.squat_switch		= SW_OPEN;
	A320.cockpit_lights		= LIT_RED;
	print_state(&A320);

	char input[INPUT_SIZE] = {0};
	int epwait_monitor; //used to verify if the squat switch was triggered

	/* epoll() setup stage 1: create an epoll instance in kernelspace that lets
	 * us keep a track of one file descriptor (epoll_create(1)), which will be
	 * stdin. The epoll instance is in the form of a set data structure.
	 */
	int epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		fprintf(stderr, "epoll_create() failure.\n");
		return REF_INACTIVE;
	}

	/* stage 2: create an epoll struct that will contain details of what we want
	 * to watch and what we're watching it for.
	 */
	struct epoll_event ep_event;
	ep_event.events = EPOLLIN; //we're watching for input activity
	ep_event.data.fd = 0; //from stdin

	/* stage 3: interface with epoll_ctl to notify the kernel that we have an
	 * epoll file descriptor (epoll_fd), an event struct (ep_event) and that we
	 * want to watch stdin (0) for activity (EPOLL_CTL_ADD).
	 */
	int epctl_test = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &ep_event);
	if (epctl_test)
	{
		fprintf(stderr, "epoll_ctl() failure.\n");
		close(epoll_fd);
		return REF_INACTIVE;
	}
	/* We should now be able to time stdin for activity: if there is activity
	 * activity reported by epoll_wait then our pilot's lever or squat switch
	 * have been triggered again, so takeoff should be aborted. Otherwise the
	 * landing gear should be retracted for a normal takeoff.
	 */

	printf("Monitoring squat switch and pilot's lever.\n");
	printf("[5 seconds to abort takeoff (\"Enter\" aborts.)]\n");
	/* epoll_wait returns an integer > 0 if input was detected because that is
	 * the number of file descriptors that are now ready for I/O operations. It
	 * returns 0 if no file descriptors are ready, implying that no activity was
	 * detected from any of the registered ones (in this case, stdin.) Therefore
	 * if user input was detected, it means takeoff has to be aborted.
	 */
	epwait_monitor = epoll_wait(epoll_fd, &ep_event, 1, WAIT);
	if (epwait_monitor > 0)
	{
		printf("Input received, aborting takeoff...");
		read(ep_event.data.fd, input, INPUT_SIZE);
		close(epoll_fd);
		current_state = STATE_GROUND;
		return REF_ACTIVATE;
	}
	/* On the other hand if the controls are untouched, the landing gear has to
	 * be retracted before preparing for an altitudinal climb.
	 */
	else if (epwait_monitor == 0)
	{
		printf("\nTakeoff committed.");
		close(epoll_fd);
		current_state = STATE_ASCEND;
		return REF_ACTIVATE;
	}
	//However, if in some case -1 was returned, we need to handle that as well.
	else
	{
		fprintf(stderr, "epoll_wait() error.\n");
		close(epoll_fd);
		return REF_INACTIVE;
	}
} //end uint8_t craft_takeoff()
