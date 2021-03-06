ir_stats_source = config.c main.c server.c daemonize.c db.c ini.c
ir_stats_headers = config.h $(top_srcdir)/valid.h

bin_cflags = $(COVERAGE_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/inc
bin_ldflags = $(COVERAGE_LDFLAGS)

if ENABLE_STANDALONE

bin_PROGRAMS = ir_stats
standalone_cflags = -DIR_STATS_STANDALONE=1

ir_stats_SOURCES = $(ir_stats_source) $(ir_stats_headers)
ir_stats_LDFLAGS = $(bin_ldflags)
ir_stats_CFLAGS = $(bin_cflags) $(standalone_cflags)

endif # ENABLE_STANDALONE

if ENABLE_LIBRARY

lib_LTLIBRARIES = libir-stats.la
library_cflags = -DIR_STATS_LIBRARY=1

libir_stats_la_SOURCES = $(ir_stats_source)
libir_stats_la_CFLAGS = $(bin_cflags) $(library_cflags)
libir_stats_la_LDFLAGS = $(bin_ldflags) -version-info 1:0:1

endif # ENABLE_LIBRARY
# static code analyzer

if ENABLE_ANALYZER

analyze_plists = $(ir_stats_source:%.c=%.plist)
MOSTLYCLEANFILES = $(analyze_plists)

$(analyze_plists): %.plist: %.c
	clang --analyze -DHAVE_CONFIG_H -I../inc -I. -I.. $< -o -cconfig.file.ir_stats

analyze: $(analyze_plists)

endif # ENABLE_ANALYZER
