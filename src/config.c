/* ==========================================================================
    !!! File generated with progen project. Any changes will overwritten !!!
   ==========================================================================
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

#if HAVE_CONFIG_H
#	include "ir-stats-config.h"
#endif

#if IR_STATS_ENABLE_INI
#	include <ini.h>
#endif

#if IR_STATS_ENABLE_GETOPT
#	include <unistd.h>
#endif

#if IR_STATS_ENABLE_GETOPT_LONG
#	include <unistd.h>
#	include <getopt.h>
#endif

#include <embedlog.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if IR_STATS_ENABLE_GETOPT && IR_STATS_ENABLE_GETOPT_LONG
#	error Both IR_STATS_ENABLE_GETOPT and IR_STATS_ENABLE_GETOPT_LONG, \
you can set only one
#endif


/* ==========================================================================
          __             __                     __   _
     ____/ /___   _____ / /____ _ _____ ____ _ / /_ (_)____   ____   _____
    / __  // _ \ / ___// // __ `// ___// __ `// __// // __ \ / __ \ / ___/
   / /_/ //  __// /__ / // /_/ // /   / /_/ // /_ / // /_/ // / / /(__  )
   \__,_/ \___/ \___//_/ \__,_//_/    \__,_/ \__//_/ \____//_/ /_//____/

   ========================================================================== */

/* declarations to make life easier when parsing same thing over
 * and over again
 */

#define PARSE_INT(OPTNAME, OPTARG, MINV, MAXV) \
	{ \
		long   val;	 /* value converted from name */ \
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ \
		\
		if (ir_stats_config_get_number(OPTARG, &val) != 0) \
		{ \
			fprintf(stderr, "%s, invalid number %s", #OPTNAME, OPTARG); \
			return -1; \
		} \
		\
		if (val < MINV || MAXV < val) \
		{ \
			fprintf(stderr, "%s: bad value %s(%ld), min: %ld, max: %ld\n", \
					#OPTNAME, OPTARG, (long)val, (long)MINV, (long)MAXV); \
			return -1; \
		} \
		g_ir_stats_config.OPTNAME = val; \
	}

#define VALID_STR(OPTNAME, OPTARG) \
	size_t optlen; \
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ \
	\
	optlen = strlen(OPTARG); \
	\
	if (optlen >= sizeof(g_ir_stats_config.OPTNAME)) \
	{ \
		fprintf(stderr, "%s: is too long %s(%ld),  max: %ld\n", \
				#OPTNAME, optarg, (long)optlen, \
				(long)sizeof(g_ir_stats_config.OPTNAME)); \
		return -1; \
	}

#define PARSE_MAP(OPTNAME, MAPNAME) \
	{ \
		long long  val; \
		if (ir_stats_config_parse_map(MAPNAME, \
					&val, g_ir_stats_ ## OPTNAME ## _map) != 0) \
			return -1; \
		g_ir_stats_config.OPTNAME = val; \
	}

#define PARSE_STR(OPTNAME, OPTARG) \
	{ \
		VALID_STR(OPTNAME, OPTARG); \
		strcpy(g_ir_stats_config.OPTNAME, OPTARG); \
	}

#define PARSE_INT_INI_NS(OPTNAME, MINV, MAXV) \
	PARSE_INT(OPTNAME, value, MINV, MAXV)

#define PARSE_INT_INI(SECTION, OPTNAME, MINV, MAXV) \
	PARSE_INT(SECTION ## _ ## OPTNAME, value, MINV, MAXV)

#define PARSE_STR_INI_NS(OPTNAME) \
	PARSE_STR(OPTNAME, value)

#define PARSE_STR_INI(SECTION, OPTNAME) \
	PARSE_STR(SECTION ## _ ## OPTNAME, value)

#define PARSE_MAP_INI_NS(OPTNAME) \
	PARSE_MAP(OPTNAME, value)

#define PARSE_MAP_INI(SECTION, OPTNAME) \
	PARSE_MAP(SECTION ## _ ## OPTNAME, value)

#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG

/* list of short options for getopt_long */
static const char *shortopts = ":hvc:o:Du:g:P:i:p:l:s:b:";

#endif


/* array of long options for getop_long. This is defined as macro so it
 * can be used in functions scope. If we would have delacred it as static
 * here, then it would take that memory for the whole duration of app,
 * and there is no need for it - instead it will take memory only in
 * functions' stack */

#define STRUCT_OPTION_LONGOPTS \
	const struct option longopts[] = \
	{ \
		{"help",               no_argument,       NULL, 'h'}, \
		{"version",            no_argument,       NULL, 'v'}, \
		{"config",             required_argument, NULL, 'c'}, \
		{"output-dir",         required_argument, NULL, 'o'}, \
		{"daemonize",          no_argument,       NULL, 'D'}, \
		{"user",               required_argument, NULL, 'u'}, \
		{"group",              required_argument, NULL, 'g'}, \
		{"pid-file",           required_argument, NULL, 'P'}, \
		{"driver-id",          required_argument, NULL, 'i'}, \
		{"log-path",           required_argument, NULL, 'p'}, \
		{"log-level",          required_argument, NULL, 'l'}, \
		{"log-frotate-number", required_argument, NULL, 256}, \
		{"log-colors",         no_argument,       NULL, 257}, \
		{"log-fsync-level",    required_argument, NULL, 258}, \
		{"log-frotate-size",   required_argument, NULL, 259}, \
		{"log-fsync-every",    required_argument, NULL, 260}, \
		{"log-ts",             required_argument, NULL, 261}, \
		{"log-ts-tm",          required_argument, NULL, 262}, \
		{"log-ts-tm-fract",    required_argument, NULL, 263}, \
		{"log-finfo",          no_argument,       NULL, 264}, \
		{"log-funcinfo",       no_argument,       NULL, 265}, \
		{"log-output",         required_argument, NULL, 266}, \
		{"log-prefix",         required_argument, NULL, 267}, \
		{"server-port",        required_argument, NULL, 's'}, \
		{"server-bind-ip",     required_argument, NULL, 'b'}, \
 \
		{NULL, 0, NULL, 0} \
	}

struct ir_stats_config_map
{
	const char  *str;
	long long    val;
};

/* define config object in static storage duration - c'mon, you
 * won't be passing pointer to config to every function, will you?
 */

static struct ir_stats_config  g_ir_stats_config;

/* define constant pointer to config object - it will be
 * initialized in init() function. It's const because you really
 * shouldn't modify config once it's set, if you need mutable
 * config, you will be better of creating dedicated module for that
 * and store data in /var/lib instead. This config module should
 * only be used with configs from /etc which should be readonly by
 * the programs
 */

const struct ir_stats_config  *ir_stats_config;

/* arrays of strings of options that have mapped values, used
 * to map config ints back into strings for nice config_dump()
 */

static const struct ir_stats_config_map g_ir_stats_log_level_map[] =
{
	{ "fatal", 0 },
	{ "alert", 1 },
	{ "crit", 2 },
	{ "error", 3 },
	{ "warn", 4 },
	{ "notice", 5 },
	{ "info", 6 },
	{ "dbg", 7 },
	{ NULL, 0 }
};

static const struct ir_stats_config_map g_ir_stats_log_ts_map[] =
{
	{ "off", EL_TS_OFF },
	{ "short", EL_TS_SHORT },
	{ "long", EL_TS_LONG },
	{ NULL, 0 }
};

static const struct ir_stats_config_map g_ir_stats_log_ts_tm_map[] =
{
	{ "clock", EL_TS_TM_CLOCK },
	{ "time", EL_TS_TM_TIME },
	{ "realtime", EL_TS_TM_REALTIME },
	{ "monotonic", EL_TS_TM_MONOTONIC },
	{ NULL, 0 }
};

static const struct ir_stats_config_map g_ir_stats_log_ts_tm_fract_map[] =
{
	{ "off", EL_TS_FRACT_OFF },
	{ "ms", EL_TS_FRACT_MS },
	{ "us", EL_TS_FRACT_US },
	{ "ns", EL_TS_FRACT_NS },
	{ NULL, 0 }
};

static const struct ir_stats_config_map g_ir_stats_log_output_map[] =
{
	{ "stderr", EL_OUT_STDERR },
	{ "stdout", EL_OUT_STDOUT },
	{ "file", EL_OUT_FILE },
	{ NULL, 0 }
};


/* ==========================================================================
                  _                __           ____
    ____   _____ (_)_   __ ____ _ / /_ ___     / __/__  __ ____   _____ _____
   / __ \ / ___// /| | / // __ `// __// _ \   / /_ / / / // __ \ / ___// ___/
  / /_/ // /   / / | |/ // /_/ // /_ /  __/  / __// /_/ // / / // /__ (__  )
 / .___//_/   /_/  |___/ \__,_/ \__/ \___/  /_/   \__,_//_/ /_/ \___//____/
/_/
   ========================================================================== */


/* ==========================================================================
    Parses map of strings and if arg is found in map, it's numerical value
    (map index) is stored in field
   ========================================================================== */


static int ir_stats_config_parse_map
(
	const char  *aargs,   /* string(s) to search in a map */
	long long   *field,   /* numerical value for arg will be stored here */
	const struct ir_stats_config_map  *map
)
{
	const struct ir_stats_config_map  *maps;
	char         args[43];
	char        *s;
	char        *sp;
	int          ret;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	maps = map;
	strcpy(args, aargs);
	*field = 0;

	for (s = strtok_r(args, ",", &sp); s != NULL; s = strtok_r(NULL, ",", &sp))
	{
		for (ret = -1, map = maps; map->str != NULL; ++map)
		{
			if (strcmp(map->str, s) == 0)
			{
				*field += map->val;
				ret = 0;
			}
		}

		if (ret == -1)
		{
			fprintf(stderr, "error, invalid option: %s\n", s);
			return -1;
		}
	}

	return ret;
}


/* ==========================================================================
    Parses map of strings and if numerical arg is found in map, pointer to
    string for that value is returned, or NULL if not found.
   ========================================================================== */


static const char * ir_stats_config_parse_map_by_value
(
	long long    val,     /* value to search in a map */
	int          flag,    /* is val flag type or single value? */
	char        *ret,     /* buf to store flag strings, used when flag == 1 */
	const struct ir_stats_config_map  *map
)
{
	if (flag == 0)
		for (; map->str != NULL; ++map)
			if (map->val == val)
				return map->str;

	ret[0] = '\0';
	for (; map->str != NULL; ++map)
		if (val & map->val)
		{
			strcat(ret, map->str);
			strcat(ret, ",");
		}

	/* remove trailing comma */
	ret[strlen(ret) - 1] = '\0';
	return ret;
}



/* ==========================================================================
    Guess what! It prints help into stdout! Would you belive that?
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG

static int ir_stats_config_print_help
(
	const char  *name  /* program name (argv[0]) */
)
{
	printf(
"Usage: %s [-h | -v | options]\n"
"\n"
"options:\n"
"\t-h, --help                        print this help and exit\n"
"\t-v, --version                     print version information and exit\n"
"\t-c, --config=<file>               config file to use\n"
"\t-o, --output-dir=<path>           location where ir-stats keeps its database\n"
"\t-D, --daemonize                   run as daemon\n"
"\t-u, --user=<user>                 name of user that should run daemon (only with -D)\n"
"\t-g, --group=<user>                name of group that should run daemon (only with -D)\n"
"\t-P, --pid-file=<path>             where to store daemon pid file (only with -D)\n"
"\t-i, --driver-id=<id>              driver (customer) id\n"
"\t-p, --log-path=<path>             location where to store program logs\n"
"\t-l, --log-level=<level>           maximum log level to print\n"
"\t    --log-frotate-number=<num>    max number of files to rotate (0 to disable)\n"
"\t    --log-colors                  add ascii colors to logs depending on level printed\n"
"\t    --log-fsync-level=<level>     minimum level of log that should always be synced\n"
"\t    --log-frotate-size=<size>     maximum size single log file can get\n"
"\t    --log-fsync-every=<size>      log will be synced to drive when this ammount of bytes have been written\n"
"\t    --log-ts=<ts>                 timestamp format to add to each log message\n"
"\t    --log-ts-tm=<tm>              source of the clock to use for timestamping\n"
"\t    --log-ts-tm-fract=<fract>     level of fraction of seconds detail to print\n"
"\t    --log-finfo                   add filename to every print\n"
"\t    --log-funcinfo                add function name to every print\n"
"\t    --log-output=<output>         outputs to enable for printing\n"
"\t    --log-prefix=<prefix>         string to prefix each log print with\n"
"\t-s, --server-port=<port>          port to listen on\n"
"\t-b, --server-bind-ip=<ip>         ip address to bind to\n"

, name);
}

#endif /* IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG */


/* ==========================================================================
    Print version and author of the program. And what else did you thing
    this function might do?
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG

static void ir_stats_config_print_version
(
	void
)
{
	printf("ir_stats "PACKAGE_VERSION"\n"
			"by Michał Łyszczek <michal.lyszczek@bofc.pl>\n");
}

#endif /* IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG */


/* ==========================================================================
    Converts string number 'num' into number representation. Converted value
    will be stored in address pointed by 'n'.
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG || IR_STATS_ENABLE_INI

static int ir_stats_config_get_number
(
	const char  *num,  /* string to convert to number */
	long        *n     /* converted num will be placed here */
)
{
	const char  *ep;   /* endptr for strtol function */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	if (*num == '\0')
	{
		errno = EINVAL;
		return -1;
	}

	*n = strtol(num, (char **)&ep, 10);

	if (*ep != '\0')
	{
		errno = EINVAL;
		return -1;
	}

	if (*n == LONG_MAX || *n == LONG_MIN)
	{
		errno = ERANGE;
		return -1;
	}

	return 0;
}

#endif /* IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG || IR_STATS_ENABLE_INI */

/* ==========================================================================
    Parse arguments passed from ini file. This funciton is called by inih
    each time it reads valid option from file.
   ========================================================================== */


#if IR_STATS_ENABLE_INI


static int ir_stats_config_parse_ini
(
	void        *user,     /* user pointer - not used */
	const char  *section,  /* section name of current option */
	const char  *name,     /* name of current option */
	const char  *value,    /* value of current option */
	int          lineno    /* line of current option */
)
{
	(void)user;
	(void)lineno;

	/* parsing section  */

	if (strcmp(section, "") == 0)
    {
		if (strcmp(name, "output-dir") == 0)
			PARSE_STR_INI_NS(output_dir)
		else if (strcmp(name, "daemonize") == 0)
			PARSE_INT_INI_NS(daemonize, 0, 1)
		else if (strcmp(name, "user") == 0)
			PARSE_STR_INI_NS(user)
		else if (strcmp(name, "group") == 0)
			PARSE_STR_INI_NS(group)
		else if (strcmp(name, "pid-file") == 0)
			PARSE_STR_INI_NS(pid_file)
		else if (strcmp(name, "driver-id") == 0)
			PARSE_STR_INI_NS(driver_id)
	}

	/* parsing section log */

	else if (strcmp(section, "log") == 0)
    {
		if (strcmp(name, "path") == 0)
			PARSE_STR_INI(log, path)
		else if (strcmp(name, "level") == 0)
			PARSE_MAP_INI(log, level)
		else if (strcmp(name, "frotate-number") == 0)
			PARSE_INT_INI(log, frotate_number, 0, INT_MAX)
		else if (strcmp(name, "colors") == 0)
			PARSE_INT_INI(log, colors, 0, 1)
		else if (strcmp(name, "fsync-level") == 0)
			PARSE_INT_INI(log, fsync_level, 0, 7)
		else if (strcmp(name, "frotate-size") == 0)
			PARSE_INT_INI(log, frotate_size, 0, SIZE_MAX)
		else if (strcmp(name, "fsync-every") == 0)
			PARSE_INT_INI(log, fsync_every, 0, SIZE_MAX)
		else if (strcmp(name, "ts") == 0)
			PARSE_MAP_INI(log, ts)
		else if (strcmp(name, "ts-tm") == 0)
			PARSE_MAP_INI(log, ts_tm)
		else if (strcmp(name, "ts-tm-fract") == 0)
			PARSE_MAP_INI(log, ts_tm_fract)
		else if (strcmp(name, "finfo") == 0)
			PARSE_INT_INI(log, finfo, 0, 1)
		else if (strcmp(name, "funcinfo") == 0)
			PARSE_INT_INI(log, funcinfo, 0, 1)
		else if (strcmp(name, "output") == 0)
			PARSE_MAP_INI(log, output)
		else if (strcmp(name, "prefix") == 0)
			PARSE_STR_INI(log, prefix)
	}

	/* parsing section server */

	else if (strcmp(section, "server") == 0)
    {
		if (strcmp(name, "port") == 0)
			PARSE_INT_INI(server, port, 0, 65535)
		else if (strcmp(name, "bind-ip") == 0)
			PARSE_STR_INI(server, bind_ip)
	}

	/* as far as inih is concerned, 1 is OK, while 0 would be error */
	return 1;
}

#endif /* IR_STATS_ENABLE_INI */


/* ==========================================================================
    Parse arguments passed from command line using getopt_long
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT_LONG

static int ir_stats_config_parse_args
(
	int    argc,
	char  *argv[]
)
{
	int    arg;      /* current option being parsed */
	int    loptind;  /* current long option index */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	STRUCT_OPTION_LONGOPTS;
	optind = 1;
	while ((arg = getopt_long(argc, argv, shortopts, longopts, &loptind)) != -1)
	{
		switch (arg)
		{
		case 'h': ir_stats_config_print_help(argv[0]); return -2;
		case 'v': ir_stats_config_print_version(); return -3;
		case 'c': /* already parsed, ignore */; break;
		case 'o': PARSE_STR(output_dir, optarg); break;
		case 'D': g_ir_stats_config.daemonize = 1; break;
		case 'u': PARSE_STR(user, optarg); break;
		case 'g': PARSE_STR(group, optarg); break;
		case 'P': PARSE_STR(pid_file, optarg); break;
		case 'i': PARSE_STR(driver_id, optarg); break;
		case 'p': PARSE_STR(log_path, optarg); break;
		case 'l': PARSE_MAP(log_level, optarg); break;
		case 256: PARSE_INT(log_frotate_number, optarg, 0, INT_MAX); break;
		case 257: g_ir_stats_config.log_colors = 1; break;
		case 258: PARSE_INT(log_fsync_level, optarg, 0, 7); break;
		case 259: PARSE_INT(log_frotate_size, optarg, 0, SIZE_MAX); break;
		case 260: PARSE_INT(log_fsync_every, optarg, 0, SIZE_MAX); break;
		case 261: PARSE_MAP(log_ts, optarg); break;
		case 262: PARSE_MAP(log_ts_tm, optarg); break;
		case 263: PARSE_MAP(log_ts_tm_fract, optarg); break;
		case 264: g_ir_stats_config.log_finfo = 1; break;
		case 265: g_ir_stats_config.log_funcinfo = 1; break;
		case 266: PARSE_MAP(log_output, optarg); break;
		case 267: PARSE_STR(log_prefix, optarg); break;
		case 's': PARSE_INT(server_port, optarg, 0, 65535); break;
		case 'b': PARSE_STR(server_bind_ip, optarg); break;


		case ':':
			fprintf(stderr, "option -%c, --%s requires an argument\n",
				optopt, longopts[loptind].name);
			return -1;

		case '?':
			fprintf(stderr, "unknown option -%c (0x%02x)\n", optopt, optopt);
			return -1;

		default:
			fprintf(stderr, "unexpected return from getopt 0x%02x\n", arg);
			return -1;
		}
	}

	return 0;
}

#endif /* IR_STATS_ENABLE_GETOPT_LONG */


/* ==========================================================================
    Parse arguments passed from command line using getopt - no long options
    supported here, if option does not have short opt, use won't be able
    to set it via command line.
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT

static int ir_stats_config_parse_args
(
	int    argc,   /* number of arguments in argv */
	char  *argv[]  /* program arguments */
)
{
	int    arg;  /* current option being parsed */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	optind = 1;
	while ((arg = getopt(argc, argv, shortopts)) != -1)
	{
		switch (arg)
		{
		case 'h': ir_stats_config_print_help(argv[0]); return -2;
		case 'v': ir_stats_config_print_version(); return -2;
		case 'c': /* already parsed, ignore */; break;
		case 'o': PARSE_STR(output_dir, optarg); break;
		case 'D': g_ir_stats_config.daemonize = 1; break;
		case 'u': PARSE_STR(user, optarg); break;
		case 'g': PARSE_STR(group, optarg); break;
		case 'P': PARSE_STR(pid_file, optarg); break;
		case 'i': PARSE_STR(driver_id, optarg); break;
		case 'p': PARSE_STR(log_path, optarg); break;
		case 'l': PARSE_MAP(log_level, optarg); break;
		case 's': PARSE_INT(server_port, optarg, 0, 65535); break;
		case 'b': PARSE_STR(server_bind_ip, optarg); break;


		case ':':
			fprintf(stderr, "option -%c requires an argument\n", optopt);
			return -1;

		case '?':
			fprintf(stderr, "unknown option -%c (0x%02x)\n", optopt, optopt);
			return -1;

		default:
			fprintf(stderr, "unexpected return from getopt 0x%02x\n", arg);
			return -1;
		}
	}

	return 0;
}

#endif /* IR_STATS_ENABLE_GETOPT */


/* ==========================================================================
                       __     __ _          ____
        ____   __  __ / /_   / /(_)_____   / __/__  __ ____   _____ _____
       / __ \ / / / // __ \ / // // ___/  / /_ / / / // __ \ / ___// ___/
      / /_/ // /_/ // /_/ // // // /__   / __// /_/ // / / // /__ (__  )
     / .___/ \__,_//_.___//_//_/ \___/  /_/   \__,_//_/ /_/ \___//____/
    /_/
   ========================================================================== */



/* ==========================================================================
    Parses options in following order (privided that parsing was enabled
    during compilation).

    - set option values to their well-known default values
    - if proper c define (-D) is enabled, overwrite that option with it
    - overwrite options specified in ini
    - overwrite options passed by command line
   ========================================================================== */


int ir_stats_config_init
(
	int    argc,   /* number of arguments in argv */
	char  *argv[]  /* program arguments from command line */
)
{
#if IR_STATS_ENABLE_INI
	const char  *file;   /* path to ini config file */
	int          arg;    /* current argument from getopt() */
#endif
#if IR_STATS_ENABLE_GETOPT_LONG
	STRUCT_OPTION_LONGOPTS;
#endif
	int          ret;    /* function return value */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	(void)argc;
	(void)argv;
	ret = 0;
	memset(&g_ir_stats_config, 0x00, sizeof(g_ir_stats_config));

	/* disable error printingfrom getopt library, some getopt() libraries
	 * (like the one in nuttx) don't support opterr, so we add define to
	 * disable this set. By default it is enabled though. To disable it
	 * just pass -DIR_STATS_NO_OPTERR to compiler.
	 */

#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG
#	ifndef IR_STATS_NO_OPTERR
	opterr = 0;
#	endif
#endif

	/* set config with default, well known values */

	strcpy(g_ir_stats_config.output_dir, "/var/lib/ir-stats");
	g_ir_stats_config.daemonize = 0;
	strcpy(g_ir_stats_config.user, "ir-stats");
	strcpy(g_ir_stats_config.group, "ir-stats");
	strcpy(g_ir_stats_config.pid_file, "/var/run/ir-stats.pid");
	strcpy(g_ir_stats_config.driver_id, "1337");
	strcpy(g_ir_stats_config.log_path, "/var/log/ir-stats.log");
	PARSE_MAP(log_level, "info");
	g_ir_stats_config.log_frotate_number = 0;
	g_ir_stats_config.log_colors = 1;
	g_ir_stats_config.log_fsync_level = 1;
	g_ir_stats_config.log_frotate_size = 10485760;
	g_ir_stats_config.log_fsync_every = 4096;
	PARSE_MAP(log_ts, "long");
	PARSE_MAP(log_ts_tm, "realtime");
	PARSE_MAP(log_ts_tm_fract, "ms");
	g_ir_stats_config.log_finfo = 1;
	g_ir_stats_config.log_funcinfo = 1;
	PARSE_MAP(log_output, "stderr");
	strcpy(g_ir_stats_config.log_prefix, "ir-stats: ");
	g_ir_stats_config.server_port = 7454;
	strcpy(g_ir_stats_config.server_bind_ip, "0.0.0.0");

	/* overwrite values with those define in compiletime */

#ifdef IR_STATS_CONFIG_OUTPUT_DIR
	g_ir_stats_config.output_dir = IR_STATS_CONFIG_OUTPUT_DIR;
#endif

#ifdef IR_STATS_CONFIG_DAEMONIZE
	g_ir_stats_config.daemonize = IR_STATS_CONFIG_DAEMONIZE;
#endif

#ifdef IR_STATS_CONFIG_USER
	g_ir_stats_config.user = IR_STATS_CONFIG_USER;
#endif

#ifdef IR_STATS_CONFIG_GROUP
	g_ir_stats_config.group = IR_STATS_CONFIG_GROUP;
#endif

#ifdef IR_STATS_CONFIG_PID_FILE
	g_ir_stats_config.pid_file = IR_STATS_CONFIG_PID_FILE;
#endif

#ifdef IR_STATS_CONFIG_DRIVER_ID
	g_ir_stats_config.driver_id = IR_STATS_CONFIG_DRIVER_ID;
#endif

#ifdef IR_STATS_CONFIG_LOG_PATH
	g_ir_stats_config.log_path = IR_STATS_CONFIG_LOG_PATH;
#endif

#ifdef IR_STATS_CONFIG_LOG_LEVEL
	PARSE_MAP(log_level, IR_STATS_CONFIG_LOG_LEVEL);
#endif

#ifdef IR_STATS_CONFIG_LOG_FROTATE_NUMBER
	g_ir_stats_config.log_frotate_number = IR_STATS_CONFIG_LOG_FROTATE_NUMBER;
#endif

#ifdef IR_STATS_CONFIG_LOG_COLORS
	g_ir_stats_config.log_colors = IR_STATS_CONFIG_LOG_COLORS;
#endif

#ifdef IR_STATS_CONFIG_LOG_FSYNC_LEVEL
	g_ir_stats_config.log_fsync_level = IR_STATS_CONFIG_LOG_FSYNC_LEVEL;
#endif

#ifdef IR_STATS_CONFIG_LOG_FROTATE_SIZE
	g_ir_stats_config.log_frotate_size = IR_STATS_CONFIG_LOG_FROTATE_SIZE;
#endif

#ifdef IR_STATS_CONFIG_LOG_FSYNC_EVERY
	g_ir_stats_config.log_fsync_every = IR_STATS_CONFIG_LOG_FSYNC_EVERY;
#endif

#ifdef IR_STATS_CONFIG_LOG_TS
	PARSE_MAP(log_ts, IR_STATS_CONFIG_LOG_TS);
#endif

#ifdef IR_STATS_CONFIG_LOG_TS_TM
	PARSE_MAP(log_ts_tm, IR_STATS_CONFIG_LOG_TS_TM);
#endif

#ifdef IR_STATS_CONFIG_LOG_TS_TM_FRACT
	PARSE_MAP(log_ts_tm_fract, IR_STATS_CONFIG_LOG_TS_TM_FRACT);
#endif

#ifdef IR_STATS_CONFIG_LOG_FINFO
	g_ir_stats_config.log_finfo = IR_STATS_CONFIG_LOG_FINFO;
#endif

#ifdef IR_STATS_CONFIG_LOG_FUNCINFO
	g_ir_stats_config.log_funcinfo = IR_STATS_CONFIG_LOG_FUNCINFO;
#endif

#ifdef IR_STATS_CONFIG_LOG_OUTPUT
	PARSE_MAP(log_output, IR_STATS_CONFIG_LOG_OUTPUT);
#endif

#ifdef IR_STATS_CONFIG_LOG_PREFIX
	g_ir_stats_config.log_prefix = IR_STATS_CONFIG_LOG_PREFIX;
#endif

#ifdef IR_STATS_CONFIG_SERVER_PORT
	g_ir_stats_config.server_port = IR_STATS_CONFIG_SERVER_PORT;
#endif

#ifdef IR_STATS_CONFIG_SERVER_BIND_IP
	g_ir_stats_config.server_bind_ip = IR_STATS_CONFIG_SERVER_BIND_IP;
#endif


#if IR_STATS_ENABLE_INI

	/* next, process .ini file */

#	if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG

	/* custom file can only be passed via command line if command
	 * line parsing is enabled, right?
	 */

	arg = -1;
	optind = 1;
	file = IR_STATS_CONFIG_PATH_DEFAULT;

	/* we need to scan arg list to check if user provided custom
	 * file path. Even though we only look for option "-c" we need
	 * to use full shortopts with all expected arguments or bug may
	 * occur. getopt(3) can take multiple arguments (flags) with
	 * single '-', like -arg, where a, r and g are different flags
	 * that also could be specified like -a -r -g.  This is for
	 * convinience, but if not carefull can lead to tricky bug.
	 * Consider user argument "-lnotice" where -l is option to
	 * determing log level and notice is its argument.  Now
	 * shortopts is only "c:" so getopt(3) does not know that
	 * -l takes an argument and it automatically assumes it does
	 * not. So getopt(3) will check for -l and that will return
	 * unknown option error, then it will check 'n' (yes!), o, t,
	 * i and then "c". At this point getopt(3) knows that "c"
	 * takes argument which in this case will be "e". To avoid
	 * that, we getopt(3) need to know that "-l" takes an
	 * argument, and for that we need to pass every option we
	 * support to shortopts.
	 */

#		if IR_STATS_ENABLE_GETOPT_LONG
	while ((arg = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1)
#		else
	while ((arg = getopt(argc, argv, shortopts)) != -1)
#		endif
	{
		if (arg == 'c')
		{
			file = optarg;

			/* since we are looking only for "-c" argument, when we
			 * find it, we can stop searching
			 */

			break;
		}

		/* ignore any unknown options, time will come to parse them too */
	}

#	endif /* IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG */

	/* parse options from .ini config file, overwritting default
	 * and/or compile time options
	 */

	if (ini_parse(file, ir_stats_config_parse_ini, NULL) != 0)
	{
		/* failed to parse config file.
		 *
		 * But if ini file does not exist, do not crash the app,
		 * and let it run with default options. Only crash app
		 * when user explicitly passed config with '-c'. It is
		 * obvious then that user really wants custom option, so
		 * if they are not available, he should be notified.
		 */

		if (arg != 'c' && errno == ENOENT)
			goto not_an_error_after_all;

		fprintf(stderr, "error while parsing config file: %s: %s\n",
				file, strerror(errno));
		return -1;
	}

not_an_error_after_all:
#endif /* IR_STATS_ENABLE_INI */

#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG
	/* parse options passed from command line - these have the
	 * highest priority and will overwrite any other options
	 */

	optind = 1;
	ret = ir_stats_config_parse_args(argc, argv);
#endif

	/* all good, initialize global config pointer with config
	 * object
	 */

	ir_stats_config = (const struct ir_stats_config *)&g_ir_stats_config;
	return ret;
}



/* ==========================================================================
    Dumps content of config to place which has been configured in embedlog.

    NOTE: remember to initialize embedlog before calling it!
   ========================================================================== */


void ir_stats_config_dump
(
	void
)
{
	/* macros to make printing easier */

#define CONFIG_PRINT_FIELD(FIELD, MODIFIER) \
	el_print(ELN, "%s%s: "MODIFIER, #FIELD, padder + strlen(#FIELD), \
		g_ir_stats_config.FIELD)

#define CONFIG_PRINT_VAR(VAR, MODIFIER) \
	el_print(ELN, "%s%s: "MODIFIER, #VAR, padder + strlen(#VAR), VAR)

#define CONFIG_PRINT_MAP(FIELD, FLAG) \
	{ \
		char  buf[43]; \
		el_print(ELN, "%s%s: %s", #FIELD, padder + strlen(#FIELD), \
			ir_stats_config_parse_map_by_value(g_ir_stats_config.FIELD, \
				FLAG, buf, g_ir_stats_ ## FIELD ## _map)); \
	}

	const char *padder = "........................";
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	el_print(ELN, PACKAGE_STRING);
	el_print(ELN, "ir_stats configuration");

	CONFIG_PRINT_FIELD(output_dir, "%s");
	CONFIG_PRINT_FIELD(daemonize, "%i");
	CONFIG_PRINT_FIELD(user, "%s");
	CONFIG_PRINT_FIELD(group, "%s");
	CONFIG_PRINT_FIELD(pid_file, "%s");
	CONFIG_PRINT_FIELD(driver_id, "%s");
	CONFIG_PRINT_FIELD(log_path, "%s");
	CONFIG_PRINT_MAP(log_level, 0);
	CONFIG_PRINT_FIELD(log_frotate_number, "%i");
	CONFIG_PRINT_FIELD(log_colors, "%i");
	CONFIG_PRINT_FIELD(log_fsync_level, "%i");
	CONFIG_PRINT_FIELD(log_frotate_size, "%zu");
	CONFIG_PRINT_FIELD(log_fsync_every, "%zu");
	CONFIG_PRINT_MAP(log_ts, 0);
	CONFIG_PRINT_MAP(log_ts_tm, 0);
	CONFIG_PRINT_MAP(log_ts_tm_fract, 0);
	CONFIG_PRINT_FIELD(log_finfo, "%i");
	CONFIG_PRINT_FIELD(log_funcinfo, "%i");
	CONFIG_PRINT_MAP(log_output, 1);
	CONFIG_PRINT_FIELD(log_prefix, "%s");
	CONFIG_PRINT_FIELD(server_port, "%i");
	CONFIG_PRINT_FIELD(server_bind_ip, "%s");


#undef CONFIG_PRINT_FIELD
#undef CONFIG_PRINT_VAR
}
