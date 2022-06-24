# include "../src/fsm_practical.h"

uint8_t craft_landing(void)
{
	/* This is the final of three nexus functions that poll for user input to
	 * change the state of the aircraft. During landing, the pilots can either
	 * commit to the landing or decide to abort and ascend again. This decision
	 * is monitored by the state of the pilot's lever: if interrupted during the
	 * landing check, the aircraft will switch to the takeoff state to ensure
	 * that it is safe to begin retracting landing gear again (given the state
	 * of the squat switch and pilot's lever.) If there was no interruption, the
	 * aircraft will begin deploying the landing gear for touchdown.
	 */
	printf("Craft landing.\n");

	A320.pilot_lever		= LEV_DOWN;
	A320.direction_valve	= DVAL_DOWN;
	print_state(&A320);

	char input[INPUT_SIZE] = {0};
	int epwait_monitor = 0; //used to verify if the pilot's lever was triggered

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
	 * activity reported by epoll_wait then the pilots are aborting the landing,
	 * so the aircraft should switch to the takeoff state. Otherwise landing
	 * should proceed normally.
	 */

	printf("Monitoring pilot's lever.\n");
	printf("[5 seconds to abort landing (\"Enter\" aborts.)]\n");
	/* epoll_wait returns an integer > 0 if input was detected because that is
	 * the number of file descriptors that are now ready for I/O operations. It
	 * returns 0 if no file descriptors are ready, implying that no activity was
	 * detected from any of the registered ones (in this case, stdin.) Therefore
	 * if user input was detected, it means the pilot's lever was lifted again
	 * and the landing has to be aborted.
	 */
	epwait_monitor = epoll_wait(epoll_fd, &ep_event, 1, WAIT);
	if (epwait_monitor > 0)
	{
		printf("Input received, aborting landing...");
		read(ep_event.data.fd, input, sizeof(input));
		close(epoll_fd);
		current_state = STATE_TAKEOFF;
		return REF_ACTIVATE;
	}
	/* On the other hand if the pilot's lever was not triggered again, the craft
	 * really does need to land in which case the mechanical systems will extend
	 * the landing gear and prepare for touchdown.
	 */
	else if (epwait_monitor == 0)
	{
		printf("\nLanding committed, touchdown imminent.");
		close(epoll_fd);
		current_state = STATE_GROUND;
		return REF_ACTIVATE;
	}
	//However, if in some case -1 was returned, we need to handle that as well.
	else
	{
		fprintf(stderr, "epoll_wait() error.\n");
		close(epoll_fd);
		return REF_INACTIVE;
	}
} //end uint8_t craft_landing()
