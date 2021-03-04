check_PROGRAMS = ir_stats_test

ir_stats_test_source = main.c config.c
ir_stats_test_header = mtest.h

ir_stats_test_SOURCES = $(ir_stats_test_source) $(ir_stats_test_header)
ir_stats_test_CFLAGS = -I$(top_srcdir)/inc \
	-I$(top_srcdir)/src \
	-I$(top_srcdir) \
	$(COVERAGE_CFLAGS)

ir_stats_test_LDFLAGS = $(COVERAGE_LDFLAGS) -static
ir_stats_test_LDADD = $(top_builddir)/src/libir_stats.la

TESTS = $(check_PROGRAMS)
LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
	$(top_srcdir)/tap-driver.sh
CLEANFILES = ir_stats.log
# static code analyzer

if ENABLE_ANALYZER

analyze_plists = $(ir_stats_test_source:%.c=%.plist)
MOSTLYCLEANFILES = $(analyze_plists)

$(analyze_plists): %.plist: %.c
	clang --analyze -DHAVE_CONFIG_H -I../inc -I../src -I.. -I. $< -o $@

analyze: $(analyze_plists)

endif
