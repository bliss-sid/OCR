/*
# proc: ticks - returns the number of clock cycles (ticks) used by a process.
# proc:
# proc: ticksPerSec - returns the system definition for HZ, where 1/HZ seconds
# proc:               is a tick (HZ = 60).
*/

/* Timing stuff. This ought to be in a library */

#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

/* ticks, Jim Blue, 13-Nov-90 15:41
 *		Get number of ticks used by process.
 */
unsigned long
ticks()
{
	struct tms buff;
	
	times(&buff);
	return buff.tms_utime;
}


/* ticksPerSec 13-Nov-90 15:41
 *		Get number of ticks per second reported by times().
 */
int
ticksPerSec()
{
	return HZ;			/* from param.h */
}

