# include "../src/fsm_practical.h"

uint8_t craft_cruise(void)
{
	/* The function that sets the aircraft in a cruise state. By this point the
	 * landing gear is fully retracted, cockpit landing lights are off and the
	 * controls for the landing gear cluster are disabled to save power. The
	 * next state after this is the descent phase.
	 */
	printf("Craft cruising.\n");
	A320.limit_switch		= SW_CLOSE;
	A320.landing_gear		= GEAR_UP;
	A320.cockpit_lights		= LIT_OFF;
	print_state(&A320);

	char input[INPUT_SIZE] = {0};
	int epwait_monitor = 0; //used to verify if emergency input was received

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
	 * activity reported by epoll_wait then the pilots are engaging an emergency
	 * landing, so the aircraft should expect a descent. Otherwise the landing
	 * gear should be fully retracted before cruise.
	 */

	printf("Standby for input to commence landing.\n");
	printf("[(\"Enter\" to begin landing.)]\n");
	/* epoll_wait returns an integer > 0 if input was detected because that is
	 * the number of file descriptors that are now ready for I/O operations. It
	 * returns 0 if no file descriptors are ready, implying that no activity was
	 * detected from any of the registered ones (in this case, stdin.) Therefore
	 * if user input was detected, it means an emergency landing has to effected
	 * and landing gear deployed for a descent.
	 */
	epwait_monitor = epoll_wait(epoll_fd, &ep_event, 1, -1);
	if (epwait_monitor > 0)
	{
		printf("Landing control enabled, craft entering descent...");
		read(ep_event.data.fd, input, sizeof(input));
		close(epoll_fd);
		current_state = STATE_DESCEND;
		return REF_ACTIVATE;
	}
	/* On the other hand if no input was received, the craft can safely continue
	 * fully retracting the landing gear cluster before entering cruise.
	 */
	//However, if in some case -1 was returned, we need to handle that as well.
	else
	{
		fprintf(stderr, "epoll_wait() error.\n");
		close(epoll_fd);
		return REF_INACTIVE;
	}
} //end uint8_t craft_cruise()
