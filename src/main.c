/* ==========================================================================
    Licensed under bsd2 license See LICENSE file for more information
    Author: Michał Łyszczek <michal.lyszczek@bofc.pl>
   ==========================================================================
          _               __            __         ____ _  __
         (_)____   _____ / /__  __ ____/ /___     / __/(_)/ /___   _____
        / // __ \ / ___// // / / // __  // _ \   / /_ / // // _ \ / ___/
       / // / / // /__ / // /_/ // /_/ //  __/  / __// // //  __/(__  )
      /_//_/ /_/ \___//_/ \__,_/ \__,_/ \___/  /_/  /_//_/ \___//____/

   ========================================================================== */


#include "config.h"
#include "daemonize.h"

#include <embedlog.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


/* ==========================================================================
          __             __                     __   _
     ____/ /___   _____ / /____ _ _____ ____ _ / /_ (_)____   ____   _____
    / __  // _ \ / ___// // __ `// ___// __ `// __// // __ \ / __ \ / ___/
   / /_/ //  __// /__ / // /_/ // /   / /_/ // /_ / // /_/ // / / /(__  )
   \__,_/ \___/ \___//_/ \__,_//_/    \__,_/ \__//_/ \____//_/ /_//____/

   ========================================================================== */


static volatile int g_ir_stats_run;


/* ==========================================================================
                  _                __           ____
    ____   _____ (_)_   __ ____ _ / /_ ___     / __/__  __ ____   _____ _____
   / __ \ / ___// /| | / // __ `// __// _ \   / /_ / / / // __ \ / ___// ___/
  / /_/ // /   / / | |/ // /_/ // /_ /  __/  / __// /_/ // / / // /__ (__  )
 / .___//_/   /_/  |___/ \__,_/ \__/ \___/  /_/   \__,_//_/ /_/ \___//____/
/_/
   ========================================================================== */


/* ==========================================================================
    Trivial signal handler for SIGINT and SIGTERM, to shutdown program with
    grace.
   ========================================================================== */


static void sigint_handler
(
	int signo  /* signal that triggered this handler */
)
{
	(void)signo;

	g_ir_stats_run = 0;
}


/* ==========================================================================
                                              _
                           ____ ___   ____ _ (_)____
                          / __ `__ \ / __ `// // __ \
                         / / / / / // /_/ // // / / /
                        /_/ /_/ /_/ \__,_//_//_/ /_/

   ========================================================================== */


#if IR_STATS_ENABLE_LIBRARY
int ir_stats_main
#else
int main
#endif
(
	int    argc,   /* number of arguments in argv */
	char  *argv[]  /* array of passed arguments */
)
{
	int    ret;    /* return code from the program */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	{
		/* install SIGTERM and SIGINT signals for clean exit. */
		struct sigaction  sa;  /* signal action instructions */
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


		g_ir_stats_run = 1;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = sigint_handler;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGTERM, &sa, NULL);
	}

	/* first things first, initialize configuration of the program */
	ret = ir_stats_config_init(argc, argv);

	/* critical error occured when config was being parsed,
	 * in such case we don't want to continue.  */
	if (ret == -1)
		return 1;

	/* it was decided by config that program should not run,
	 * but we should not exit with and error. This happens
	 * when either -v or -h was passed */
	if (ret == -2)
		return 0;

	/* I am pessimist in nature, so I assume
	 * things will screw up from now on */
	ret = 1;

	/* Initialize logger. Should it fail? Fine, we care only a
	 * little bit but not that much to crash application so even if
	 * it fails we still continue with our program. Of course if
	 * logger fails program will probably fail too, but meh... one
	 * can allow himself to be optimistic from time to time. */

	if (el_init() == 0)
	{
		/* logger init succeed, configure it */
		el_option(EL_FROTATE_NUMBER, ir_stats_config->log_frotate_number);
		el_option(EL_FROTATE_SIZE, ir_stats_config->log_frotate_size);
		el_option(EL_FSYNC_EVERY, ir_stats_config->log_fsync_every);
		el_option(EL_FSYNC_LEVEL, ir_stats_config->log_fsync_level);
		el_option(EL_TS, ir_stats_config->log_ts);
		el_option(EL_TS_TM, ir_stats_config->log_ts_tm);
		el_option(EL_TS_FRACT, ir_stats_config->log_ts_tm_fract);
		el_option(EL_FINFO, ir_stats_config->log_finfo);
		el_option(EL_COLORS, ir_stats_config->log_colors);
		el_option(EL_PREFIX, ir_stats_config->log_prefix);
		el_option(EL_OUT, ir_stats_config->log_output);
		el_option(EL_LEVEL, ir_stats_config->log_level);

		if (ir_stats_config->log_output & EL_OUT_FILE)
		{
			/* we will be outputing logs to file, so we need to
			 * open file now */
			if (el_option(EL_FPATH, ir_stats_config->log_path) != 0)
			{
				/* in general embedlog will try to recover from
				 * any error that it may stumble upon (like
				 * directory does not yet exist - but will be
				 * created later, or permission is broker, but
				 * will be fixed later). That errors could be
				 * recovered from with some external help so
				 * there is no point disabling file logging.
				 * Any errors, except for these two.  In this
				 * case, disable logging to file as it is
				 * pointless, so we disable logging to file
				 * leaving other destinations intact */
				if (errno == ENAMETOOLONG || errno == EINVAL)
					el_option(EL_OUT, ir_stats_config->log_output & ~EL_OUT_FILE);

				/* print warning to stderr so it's not missed by
				 * integrator in case file output was the only
				 * output enabled */
				fprintf(stderr, "w/failed to open log file %s, %s\n",
						ir_stats_config->log_path, strerror(errno));
			}
		}
	}

	/* dump config, it's good to know what is
	 * program configuration when debugging later
	 * */
	ir_stats_config_dump();

	/* ================================= */
	/* put your initialization code here */
	/* ================================= */

	/* all resources initialized, now start main loop */

	if (ir_stats_config->daemonize)
		daemonize(ir_stats_config->pid_file, ir_stats_config->user,
				ir_stats_config->group);

	el_print(ELN, "all resources initialized, starting main loop");

	while (g_ir_stats_run)
	{
		/* ======================= */
		/* put your main code here */
		/* ======================= */
	}

	/* ========================== */
	/* put your cleanup code here */
	/* ========================== */

	ret = 0;

	el_print(ELN, "goodbye %s world!", ret ? "cruel" : "beautiful");
	el_cleanup();
	if (ir_stats_config->daemonize)
		daemonize_cleanup(ir_stats_config->pid_file);

	return ret;
}
