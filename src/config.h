/* ==========================================================================
    !!! File generated with progen project. Any changes will overwritten !!!
   ==========================================================================
    Licensed under BSD 2clause license See LICENSE file for more information
    Author: Michał Łyszczek <michal.lyszczek@bofc.pl>
   ========================================================================== */

#ifndef IR_STATS_CONFIG_H
#define IR_STATS_CONFIG_H 1

#if HAVE_CONFIG_H
#	include "ir-stats-config.h"
#endif

#include <limits.h>
#include <stddef.h>

struct ir_stats_config
{


	/* ==================================================================
	     section options
	   ================================================================== */


	/* directory where ir-stats will hold its database files
	 */
	char  output_dir[PATH_MAX + 1];

	/* run as daemon */
	int  daemonize;

	/* username program should run on, for security reason it should not
	 * be root.  This will take effect only when -D (--daemonize) options
	 * is provided
	 */
	char  user[32 + 1];

	/* group program should run on, for security reason it should not be
	 * root This will take effect only when -D (--daemonize) options is
	 * provided
	 */
	char  group[32 + 1];

	/* wher to store daemon pid file (only with -D) */
	char  pid_file[PATH_MAX + 1];


	/* ==================================================================
	    log section options
	   ================================================================== */


	/* location where to store program logs */
	char  log_path[PATH_MAX + 1];

	/* defines how much verbose program will be about what it does
	 *
	 *   fatal  - fatal errors, application cannot continue to run
	 *   alert  - major failure that needs immediate attention
	 *   crit   - critical errors
	 *   error  - error but recoverable
	 *   warn   - warnings
	 *   notice - normal message, but of high importance
	 *   info   - informations log, doesn't print that much, recommended
	 *   debug  - debug, prints a lot of not really needed things in normal usage
	 */
	int  log_level;

	/* max number of files to rotate (0 to disable) */
	int  log_frotate_number;

	/* if set, logs will contain pretty colors to quickly find
	 * information debugger might need. Note that not all terminals
	 * support it! Either 0 or 1 is supported
	 */
	int  log_colors;

	/* minimum level of log that should always be synced */
	int  log_fsync_level;

	/* maximum size single log file can get */
	size_t  log_frotate_size;

	/* log will be synced to drive when this ammount of bytes have
	 * been written
	 */
	size_t  log_fsync_every;

	/* timestamp format to add to each log message */
	int  log_ts;

	/* source of the clock to use for timestamping */
	int  log_ts_tm;

	/* level of fraction of seconds detail to print */
	int  log_ts_tm_fract;

	/* add filename to every print */
	int  log_finfo;

	/* add function name to every print */
	int  log_funcinfo;

	/* outputs to enable for printing */
	int  log_output;

	/* string to prefix each log print with */
	char  log_prefix[32 + 1];

};

extern const struct ir_stats_config  *ir_stats_config;
int ir_stats_config_init(int argc, char *argv[]);
void ir_stats_config_dump();

#endif
