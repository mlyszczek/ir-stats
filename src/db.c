/* ==========================================================================
    Licensed under BSD 2clause license See LICENSE file for more information
    Author: Michał Łyszczek <michal.lyszczek@bofc.pl>
   ==========================================================================
          _               __            __         ____ _  __
         (_)____   _____ / /__  __ ____/ /___     / __/(_)/ /___   _____
        / // __ \ / ___// // / / // __  // _ \   / /_ / // // _ \ / ___/
       / // / / // /__ / // /_/ // /_/ //  __/  / __// // //  __/(__  )
      /_//_/ /_/ \___//_/ \__,_/ \__,_/ \___/  /_/  /_//_/ \___//____/

   ========================================================================== */

#include "config.h"
#include "db.h"

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <embedlog.h>
#include <ctype.h>
#include <stdlib.h>


/* ==========================================================================
          __             __                     __   _
     ____/ /___   _____ / /____ _ _____ ____ _ / /_ (_)____   ____   _____
    / __  // _ \ / ___// // __ `// ___// __ `// __// // __ \ / __ \ / ___/
   / /_/ //  __// /__ / // /_/ // /   / /_/ // /_ / // /_/ // / / /(__  )
   \__,_/ \___/ \___//_/ \__,_//_/    \__,_/ \__//_/ \____//_/ /_//____/

   ========================================================================== */


struct frame_info  last_frame;

double  traveled_by_car;
double  traveled_by_car_on_track;


/* ==========================================================================
                  _                __           ____
    ____   _____ (_)_   __ ____ _ / /_ ___     / __/__  __ ____   _____ _____
   / __ \ / ___// /| | / // __ `// __// _ \   / /_ / / / // __ \ / ___// ___/
  / /_/ // /   / / | |/ // /_/ // /_ /  __/  / __// /_/ // / / // /__ (__  )
 / .___//_/   /_/  |___/ \__,_/ \__/ \___/  /_/   \__,_//_/ /_/ \___//____/
/_/
   ========================================================================== */


int create_dir
(
	const char  *dir  /* dir to create, relative to output_dir/driver_id */
)
{
	char  path[PATH_MAX];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (dir)
		sprintf(path, "%s/%s/%s", ir_stats_config->output_dir,
				ir_stats_config->driver_id, dir);
	else
		sprintf(path, "%s/%s", ir_stats_config->output_dir,
				ir_stats_config->driver_id);

	if (mkdir(path, 0755) != 0)
	{
		/* if directory exist, ignore, otherwise its an
		 * fatal error */
		if (errno != EEXIST)
		{
			el_perror(ELF, "mkdir(%s, 0755)", path);
			return -1;
		}
	}

	return 0;
}

/* ==========================================================================
    Updates value in file with val. Use "a" mode to append to file or "w" to
    overwrite file with new value.
   ========================================================================== */


int update_file
(
	const char  *file,
	const char  *val,
	const char  *mode
)
{
	FILE        *f;
	char         path[PATH_MAX];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	sprintf(path, "%s/%s/%s", ir_stats_config->output_dir,
			ir_stats_config->driver_id, file);

	f = fopen(path, mode);
	if (f == NULL)
	{
		el_perror(ELE, "fopen(%s, '%s')", path, mode);
		return -1;
	}

	if (fwrite(val, strlen(val), 1, f) != 1)
	{
		el_perror(ELE, "fwrite(%s, %d, 1, %s)", val, strlen(val), 1, path);
		fclose(f);
		return -1;
	}

	fwrite("\n", 1, 1, f);
	fclose(f);
	return 0;
}


/* ==========================================================================
    Updates double value in file, will overwrite content of file Use "a"
    mode to append to file or "w" to overwrite file with new value.
   ========================================================================== */


int update_file_double
(
	const char  *file,
	double       val,
	const char  *mode
)
{
	char         sval[32];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	sprintf(sval, "%f", val);
	return update_file(file, sval, mode);
}


/* ==========================================================================
   ========================================================================== */


static void dump_to_db
(
	struct frame_info *fi
)
{
	char path[PATH_MAX];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (traveled_by_car)
	{
		sprintf(path, "distance-traveled/cars/%s", fi->car);
		update_file_double(path, traveled_by_car, "w");
		el_print(ELN, "traveled by car: %f, car %s", traveled_by_car, fi->car);
	}


	if (traveled_by_car_on_track)
	{
		sprintf(path, "distance-traveled/tracks/%s", fi->track);
		create_dir(path);
		sprintf(path, "distance-traveled/tracks/%s/%s", fi->track, fi->car);
		update_file_double(path, traveled_by_car_on_track, "w");
		el_print(ELN, "traveled by car: %f, car %s, on track %s",
				traveled_by_car_on_track, fi->car, fi->track);
	}

	/* lap time can be -1, when you do first lap from the pit, let's
	 * not put that into the file as it's useless information */
	el_print(ELD, "laptime %f, prev: %f", fi->lap_time, last_frame.lap_time);
	if (fi->lap_time > 0 && fi->lap_time != last_frame.lap_time)
	{
		sprintf(path, "lap-times/%s", fi->track);
		create_dir(path);
		sprintf(path, "lap-times/%s/%s", fi->track, fi->car);
		update_file_double(path, fi->lap_time, "a");
		el_print(ELN, "lap time %f, car %s on track: %s",
				traveled_by_car, fi->car, fi->track);
	}
}


/* ==========================================================================
   ========================================================================== */


int read_from_file
(
	const char  *file,
	char        *out,
	size_t       outl
)
{
	FILE        *f;
	char         path[PATH_MAX];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	sprintf(path, "%s/%s/%s", ir_stats_config->output_dir,
			ir_stats_config->driver_id, file);
	f = fopen(path, "r");
	if (f == NULL)
	{
		el_perror(ELE, "fopen(%s, 'r')", path);
		return -1;
	}

	errno = 0;
	if (fread(out, 1, outl, f) < 1)
	{
		el_perror(ELE, "fread(%s)", path);
		fclose(f);
		return -1;
	}

	fclose(f);
	return 0;
}


/* ==========================================================================
   ========================================================================== */


double read_from_file_double
(
	const char  *file
)
{
	char         buf[64];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (read_from_file(file, buf, sizeof(buf)) != 0)
		return 0;

	return atof(buf);
}


/* ==========================================================================
   ========================================================================== */


static void load_from_db
(
	struct frame_info *fi
)
{
	char path[PATH_MAX];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	sprintf(path, "distance-traveled/cars/%s", fi->car);
	traveled_by_car = read_from_file_double(path);
	el_print(ELN, "traveled_by_car: %f, car %s", traveled_by_car, fi->car);

	sprintf(path, "distance-traveled/tracks/%s/%s", fi->track, fi->car);
	traveled_by_car_on_track = read_from_file_double(path);
	el_print(ELN, "traveled_by_car: %f, car %s on track %s",
			traveled_by_car_on_track, fi->car, fi->track);
}


/* ==========================================================================
               ____                     __   _
              / __/__  __ ____   _____ / /_ (_)____   ____   _____
             / /_ / / / // __ \ / ___// __// // __ \ / __ \ / ___/
            / __// /_/ // / / // /__ / /_ / // /_/ // / / /(__  )
           /_/   \__,_//_/ /_/ \___/ \__//_/ \____//_/ /_//____/

   ========================================================================== */


int db_init(void)
{
	int e;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	e = 0;

	/* create base directory, this may be somewhere in the
	 * /var/lib which is unwriteable for us, so ignore
	 * any error from this call */
	mkdir(ir_stats_config->output_dir, 0755);

	/* we should have write access to config->output_dir,
	 * so this time when we create dir for driver id, we
	 * check for error - this will also catch error from
	 * previous mkdir(), since if config->output_dir does
	 * not exist, then driver id dir won't get created
	 * as well */
	e |= create_dir(NULL);
	e |= create_dir("distance-traveled");
	e |= create_dir("distance-traveled/cars");
	e |= create_dir("distance-traveled/tracks");
	e |= create_dir("lap-times");
	if (e)  return -1;

	memset(&last_frame, 0x00, sizeof(last_frame));
	traveled_by_car = 0;
	traveled_by_car_on_track = 0;
	return 0;
}


/* ==========================================================================
   ========================================================================== */


void db_update
(
	struct frame_info *fi
)
{
	if (strcmp(fi->track, last_frame.track) ||
			strcmp(fi->car, last_frame.car))
	{
		el_print(ELN, "track change: cur track: %s, car: %s, prev track: %s, car: %s",
				fi->track, fi->car, last_frame.track, last_frame.car);
		/* car or track has been changed, save last
		 * reading into db file */
		dump_to_db(&last_frame);
		/* load values for new car/track set */
		load_from_db(fi);
		memcpy(&last_frame, fi, sizeof(*fi));
		return;
	}

	//el_print(ELD, "traveled: %f, stime: %f, prev stime: %f, dif: %f, speed: %f",
	//		((fi->session_time - last_frame.session_time) *fi->speed) / 3600.0,
	//		fi->session_time, last_frame.session_time,
	//		fi->session_time - last_frame.session_time,
	//		fi->speed);
	traveled_by_car += ((fi->session_time - last_frame.session_time) *
			fi->speed);
	traveled_by_car_on_track += ((fi->session_time - last_frame.session_time) *
			fi->speed);
	//el_print(ELD, "traveled total: %f", traveled_by_car);

	/* sync data to db once every lap, this is frequent enough that
	 * noone will miss part of data from when game crashes, and
	 * it will be infrequent enough to cause any machine to choke */
	el_print(ELD, "laps completed: %d:%d, laptime: %f, speed: %f",
			fi->laps_completed, last_frame.laps_completed, fi->lap_time, fi->speed);
	/* laps_completed is unreliable, it is increased when laptime
	 * does not yet change, so rely only on laptime */
	if (fi->lap_time != last_frame.lap_time)
	{
		el_print(ELN, "lap finished, dumping db to disk");
		dump_to_db(fi);
	}

	memcpy(&last_frame, fi, sizeof(*fi));
}

