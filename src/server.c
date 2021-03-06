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
#include "server.h"
#include "db.h"

#include <embedlog.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>



/* ==========================================================================
          __             __                     __   _
     ____/ /___   _____ / /____ _ _____ ____ _ / /_ (_)____   ____   _____
    / __  // _ \ / ___// // __ `// ___// __ `// __// // __ \ / __ \ / ___/
   / /_/ //  __// /__ / // /_/ // /   / /_/ // /_ / // /_/ // / / /(__  )
   \__,_/ \___/ \___//_/ \__,_//_/    \__,_/ \__//_/ \____//_/ /_//____/

   ========================================================================== */


int  sfd;  /* server socket */
int  cfd;  /* client socket */
extern volatile int g_ir_stats_run;



/* ==========================================================================
                  _                __           ____
    ____   _____ (_)_   __ ____ _ / /_ ___     / __/__  __ ____   _____ _____
   / __ \ / ___// /| | / // __ `// __// _ \   / /_ / / / // __ \ / ___// ___/
  / /_/ // /   / / | |/ // /_/ // /_ /  __/  / __// /_/ // / / // /__ (__  )
 / .___//_/   /_/  |___/ \__,_/ \__/ \___/  /_/   \__,_//_/ /_/ \___//____/
/_/
   ========================================================================== */


void canon_name
(
	char  *name
)
{
	do
	{
		*name = tolower(*name);
		if (*name == ' ') *name = '-';
		if (!isprint(*name)) *name = '\0';
	}
	while (*++name);
}


/* ==========================================================================
    Takes value from single YAML line and stores it in dest.
   ========================================================================== */


void read_value_string
(
	char  *line,
	char  *dest
)
{
	char   c;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	*dest = '\0';
	/* skip any spaces */
	while (*++line == ' ');
	/* skip chars until ':' or ' '
	 * key value separator is found */
	while ((c = *++line) && c != ':' && c != ' ');
	/* skip any spaces */
	while (*++line == ' ');
	/* if we didn't hit end of line
	 * copy value to dest */
	if (*line) strcpy(dest, line);
	canon_name(dest);
}


/* ==========================================================================
    Takes value from single YAML line, converts it to double and returns
   ========================================================================== */


double read_value_double
(
	char  *line
)
{
	char   buf[64];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	read_value_string(line, buf);
	return atof(buf);
}


/* ==========================================================================
    Processes YAML data received by client and extracts needed info into
    internal struct. At the end calls db_update() which will update stats
    database with received values.
   ========================================================================== */


void process_iracing_frame
(
	char              *frame
)
{
	char              *line;
	char               user_id_line[128];
	struct frame_info  frame_info;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define STARTS_WITH(s) !strncmp(s, line,  strlen(s))

	sprintf(user_id_line, "   UserID: %s", ir_stats_config->driver_id);

	/* frame is technically a yaml, but this yaml is super easy
	 * and really does not have any complicated, multiline entries
	 * so we can simply strtok() and strcmp() it */
	line = strtok(frame, "\n");
	do
	{
		if (STARTS_WITH(" TrackDisplayName"))
			read_value_string(line, frame_info.track);

		else if (STARTS_WITH("Speed "))
		{
			frame_info.speed = read_value_double(line);
			if (frame_info.speed < 0.1)
				frame_info.speed = 0;
		}

		else if (STARTS_WITH("SessionTime "))
			frame_info.session_time = read_value_double(line);

		else if (STARTS_WITH("LapLastLapTime "))
			frame_info.lap_time = read_value_double(line);

		else if (STARTS_WITH("LapCompleted "))
			frame_info.laps_completed = read_value_double(line);

		/* yea, yaml is super easy but when it comes to get
		 * car id. As this is taken from Drivers: and it's
		 * an array, we need quick extra loop to get car name
		 * for current driver (for configured user) */
		else if (STARTS_WITH(user_id_line))
			while ((line = strtok(NULL, "\n")))
				if (STARTS_WITH("   CarScreenName:"))
				{
					read_value_string(line, frame_info.car);
					break;
				}

	}
	while ((line = strtok(NULL, "\n")));

	db_update(&frame_info);
}

/* ==========================================================================
               ____                     __   _
              / __/__  __ ____   _____ / /_ (_)____   ____   _____
             / /_ / / / // __ \ / ___// __// // __ \ / __ \ / ___/
            / __// /_/ // / / // /__ / /_ / // /_/ // / / /(__  )
           /_/   \__,_//_/ /_/ \___/ \__//_/ \____//_/ /_//____/

   ========================================================================== */


/* ==========================================================================
    Initializes server socket for listening and all needed memory to work.
   ========================================================================== */


int server_init(void)
{
	int                 flags;  /* flags for setting socket option */
	struct sockaddr_in  srv;    /* server address to bind to */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	el_print(ELN, "create server %s:%d", ir_stats_config->server_bind_ip,
			ir_stats_config->server_port);

	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sfd < 0)
	{
		el_perror(ELF, "couldn't create server socket");
		return -1;
	}

	/* as TCP is all about reliability, after server crashes (or is
	 * restarted), kernel still keeps our server tuple in TIME_WAIT
	 * state, to make sure all connections are closed properly
	 * disallowing us to bind to that address again. We don't need
	 * such behaviour, thus we allow SO_REUSEADDR */
	flags = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags)) != 0)
	{
		el_perror(ELE, "setsockopt(SOL_SOCKET, SO_REUSEADDR)");
		close(sfd);
		/* not fatal, even if we cannot set option, we might still
		 * be able to bind, so don't crash (yet) */
	}

	/* fill server address parameters for listening */
	memset(&srv, 0, sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_port = htons(ir_stats_config->server_port);
	srv.sin_addr.s_addr = inet_addr(ir_stats_config->server_bind_ip);

	/* bind socket to srv address so it only accepts
	 * connection from this ip/interface */
	if (bind(sfd, (struct sockaddr *)&srv, sizeof(srv)) != 0)
	{
		el_perror(ELF, "bind()");
		close(sfd);
		return -1;
	}

	/* mark socket to accept incoming connections. */
	if (listen(sfd, 10) != 0)
	{
		el_perror(ELF, "listen()");
		close(sfd);
		return -1;
	}

	cfd = -1;
	return 0;
}


void server_loop_forever(void)
{
	socklen_t           clen;    /* length of 'client' variable */
	struct sockaddr_in  client;  /* address of remote client */
	char               *buf;     /* buffer for received message */
	size_t              bufsiz;  /* size of the buf */
	size_t              bufl;    /* number of bytes in buf */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* my iracing file with telemetry is 48k big, so it should be safe
	 * to set bufsiz to 128k, if that's not enough it will grow later */
	bufsiz = 128l * 1024l;
	/* yeah, we don't check for malloc because todays systems are
	 * virtually always return valid pointer, because no memory is
	 * actually allocated yet, and if we get NULL here, well... in
	 * such case system has bigger problems than our daemon */
	buf = malloc(bufsiz);

	el_print(ELN, "server initialized and started");

	while (g_ir_stats_run)
	{
		/* wait for connection on server socket */
		el_print(ELN, "waiting for connection");
		if ((cfd = accept(sfd, (struct sockaddr *)&client, &clen)) < 0)
		{
			el_perror(ELC, "accept(sfd)");
			/* sigterm received, abort */
			if (g_ir_stats_run == 0)
				return;
			continue;
		}


		el_print(ELN, "client %s connected", inet_ntoa(client.sin_addr));

		/* process data from client until it disconnects or
		 * error happens */
		while (cfd != -1)
		{
			char     sof[24];
			ssize_t  r;
			long     to_read;
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			r = read(cfd, sof, sizeof(sof));
			if (r < 0)
			{
				el_perror(ELE, "read(cfd, sof)");
				close(cfd);
				cfd = -1;
				continue;
			}

			if (r == 0)
			{
				el_print(ELN, "client closed connection");
				close(cfd);
				cfd = -1;
				break;
			}

			/* SOF is in format:
			 * IR_STATS_SOF: 000030421
			 * and number is a number of bytes that will
			 * be send by server */
			if (r != sizeof(sof))
			{
				el_print(ELE, "ignoring invalid sof:");
				el_pmemory(ELE, sof, r);
				continue;
			}

			to_read = atol(&sof[14]);
			if ((size_t)to_read >= bufsiz)
			{
				buf = realloc(buf, 2 * to_read);
				bufsiz = to_read;
			}

			/* loop until we read whole iracing telemetry file */
			bufl = 0;
			memset(buf, 0, bufsiz);
			while (to_read)
			{
				r = read(cfd, buf + bufl, to_read);
				if (r < 0)
				{
					el_perror(ELE, "read(cfd, buf); ignoring frame");
					close(cfd);
					cfd = -1;
					break;
				}

				if (r == 0)
				{
					el_print(ELN, "client closed connection");
					close(cfd);
					cfd = -1;
					break;
				}

				to_read -= r;
				bufl += r;
			}

			/* whole message read, pass it for processing */
			if (cfd != -1)
				process_iracing_frame(buf);
		}
	}
}
