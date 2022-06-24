# include "../src/set_practical.h"

void defaults()
{
	/* This function simply clears runtime flags used by the program, resetting
	 * them to a "default" state. This function is a little vestigial: originally
	 * there were flags provided for verbosity as well as choosing which of the
	 * 2 set_ops to run, if not both. However, prioritising the effects of the
	 * verbose flag (parsing "-v" before any other flag) and implementing the
	 * usage of "-u" for union only and "-i" for intersection only was slightly
	 * tedious, and their effects were also relatively uninformative for what
	 * this program aims to achieve; hence those options were abandoned.
	 */
	u_no_rr_flag_g ^= u_no_rr_flag_g;
} //end void defaults()
