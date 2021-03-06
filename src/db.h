/* ==========================================================================
    Licensed under BSD 2clause license See LICENSE file for more information
    Author: Michał Łyszczek <michal.lyszczek@bofc.pl>
   ========================================================================== */


#ifndef DB_H
#define DB_H 1

struct frame_info
{
	/* name of the track */
	char   track[128];

	/* name of the car */
	char   car[128];

	/* current car speed in m/s (meters per second) */
	double speed;

	/* current session time, seconds from
	 * the biginning of session */
	double session_time;

	/* last lap time */
	double  lap_time;

	/* number of laps completed */
	int  laps_completed;
};

void db_update(struct frame_info *fi);
int db_init(void);

#endif
