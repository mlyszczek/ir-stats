/* ==========================================================================
   !! File generated with progen project. Any changes will get overwritten !!
   ==========================================================================
    Licensed under BSD2clause license See LICENSE file for more information
    Author: Michał Łyszczek <michal.lyszczek@bofc.pl>
   ==========================================================================
          _               __            __         ____ _  __
         (_)____   _____ / /__  __ ____/ /___     / __/(_)/ /___   _____
        / // __ \ / ___// // / / // __  // _ \   / /_ / // // _ \ / ___/
       / // / / // /__ / // /_/ // /_/ //  __/  / __// // //  __/(__  )
      /_//_/ /_/ \___//_/ \__,_/ \__,_/ \___/  /_/  /_//_/ \___//____/

   ========================================================================== */


#ifdef HAVE_CONFIG_H
#   include "ir-stats-config.h"
#endif

#include "config.h"
#include "mtest.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>


/* ==========================================================================
          __             __                     __   _
     ____/ /___   _____ / /____ _ _____ ____ _ / /_ (_)____   ____   _____
    / __  // _ \ / ___// // __ `// ___// __ `// __// // __ \ / __ \ / ___/
   / /_/ //  __// /__ / // /_/ // /   / /_/ // /_ / // /_/ // / / /(__  )
   \__,_/ \___/ \___//_/ \__,_//_/    \__,_/ \__//_/ \____//_/ /_//____/

   ========================================================================== */


mt_defs_ext();
struct ir_stats_config  config;


/* ==========================================================================
                  _                __           ____
    ____   _____ (_)_   __ ____ _ / /_ ___     / __/__  __ ____   _____ _____
   / __ \ / ___// /| | / // __ `// __// _ \   / /_ / / / // __ \ / ___// ___/
  / /_/ // /   / / | |/ // /_/ // /_ /  __/  / __// /_/ // / / // /__ (__  )
 / .___//_/   /_/  |___/ \__,_/ \__/ \___/  /_/   \__,_//_/ /_/ \___//____/
/_/
   ========================================================================== */


/* ==========================================================================
    Prepares for test, called each time test is about to run (if
    mt_prepare_test has been set).
   ========================================================================== */


static void test_prepare(void)
{
    memset(&config, 0, sizeof(config));

    strcpy(config.output_dir, "/var/lib/termsend");
    strcpy(config.domain, "http://localhost");
    config.daemonize = 0;
    strcpy(config.user, "termsend");
    strcpy(config.group, "termsend");
    strcpy(config.pid_file, "/var/run/termsend.pid");
    strcpy(config.list_file, "/etc/termsend/iplist");
#if HAVE_LST
    config.list_type = 1;
#endif
    strcpy(config.list_ip_bind, "0.0.0.0");
    strcpy(config.log_query, "/var/log/termsend-query.log");
    strcpy(config.log_path, "/var/log/termsend.log");
    config.log_level = 6;
    config.log_frotate_number = 0;
    config.log_colors = 0;
    config.log_fsync_level = 1;
    config.log_frotate_size = 10485760;
    config.log_fsync_every = 4096;
    config.log_ts = 2;
    config.log_ts_tm = 2;
    config.log_ts_tm_fract = 1;
    config.log_finfo = 0;
    config.log_funcinfo = 0;
    config.log_output = 0;
    strcpy(config.log_prefix, "termsend: ");
    config.log_level_flag = 6;
    config.max_filesize = 10485760;
    config.max_connections = 10;
    config.max_timeout = 60;
    config.max_timed_timeout = 60;
    config.port_plain = 1337;
    config.port_plain_timed = 1338;
#if HAVE_SSL
    config.port_ssl = 1339;
    config.port_ssl_timed = 1340;
    strcpy(config.ssl_key_file, "/etc/termsend/termsend.key");
    strcpy(config.ssl_pass_file, "");
    strcpy(config.ssl_cert_file, "/etc/termsend/termsend.cert");
#endif

}


/* ==========================================================================
                           __               __
                          / /_ ___   _____ / /_ _____
                         / __// _ \ / ___// __// ___/
                        / /_ /  __/(__  )/ /_ (__  )
                        \__/ \___//____/ \__//____/

   ========================================================================== */


/* ==========================================================================
   ========================================================================== */


static void config_all_default(void)
{
    char  *argv[] = { "ir-stats" };
    ir_stats_config_init(1, argv);
    mt_fok(memcmp(&config, ir_stats_config, sizeof(config)));
}


/* ==========================================================================
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG

static void config_short_opts(void)
{
    char *argv[] =
    {
        "ir-stats",
        "-oytaxxlpczmuohphekngksuyrg",
        "-dhbfhjpockorcupzbsqrammuwe",
        "-D",
        "-uqrjtgwdcwkifjonfwolp",
        "-gtyidtsvsfexomsdznh",
        "-Ppsnlixceddgvzzzhkswlfficshvp",
        "-Lcnkqizxxmogjauajtenuvpqpmaiysednjkqiis",
#if HAVE_LST
        "-Tblack",
#endif
        "-bogrnjwpjcqr",
        "-qpbztwvmzxioatpfiwylzrciskijindyrkv",
        "-pxzieuhvotfld",
        "-lcrit",
        "-s20918",
        "-m30594",
        "-t20403",
        "-M31051",
        "-i27133",
        "-a64271",
#if HAVE_SSL
        "-I57554",
        "-A34394",
        "-kffqyydsaripufotkfefjvbwrpernelttojmiahri",
        "-fxgukkhszrijklea",
        "-Caaqvxxgzgouq"
#endif

    };
    int argc = sizeof(argv) / sizeof(const char *);
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    ir_stats_config_init(argc, argv);

    /* seed: 2021-03-04 21:42:58.766003 */
    strcpy(config.output_dir, "ytaxxlpczmuohphekngksuyrg");
    strcpy(config.domain, "hbfhjpockorcupzbsqrammuwe");
    config.daemonize = 1;
    strcpy(config.user, "qrjtgwdcwkifjonfwolp");
    strcpy(config.group, "tyidtsvsfexomsdznh");
    strcpy(config.pid_file, "psnlixceddgvzzzhkswlfficshvp");
    strcpy(config.list_file, "cnkqizxxmogjauajtenuvpqpmaiysednjkqiis");
#if HAVE_LST
    config.list_type = 0;
#endif
    strcpy(config.list_ip_bind, "ogrnjwpjcqr");
    strcpy(config.log_query, "pbztwvmzxioatpfiwylzrciskijindyrkv");
    strcpy(config.log_path, "xzieuhvotfld");
    config.log_level = 2;
    config.log_frotate_number = 0;
    config.log_colors = 0;
    config.log_fsync_level = 1;
    config.log_frotate_size = 10485760;
    config.log_fsync_every = 4096;
    config.log_ts = 2;
    config.log_ts_tm = 2;
    config.log_ts_tm_fract = 1;
    config.log_finfo = 0;
    config.log_funcinfo = 0;
    config.log_output = 0;
    strcpy(config.log_prefix, "termsend: ");
    config.log_level_flag = 6;
    config.max_filesize = 20918;
    config.max_connections = 30594;
    config.max_timeout = 20403;
    config.max_timed_timeout = 31051;
    config.port_plain = 27133;
    config.port_plain_timed = 64271;
#if HAVE_SSL
    config.port_ssl = 57554;
    config.port_ssl_timed = 34394;
    strcpy(config.ssl_key_file, "ffqyydsaripufotkfefjvbwrpernelttojmiahri");
    strcpy(config.ssl_pass_file, "xgukkhszrijklea");
    strcpy(config.ssl_cert_file, "aaqvxxgzgouq");
#endif


    mt_fail(strcmp(example_config->output_dir, "ytaxxlpczmuohphekngksuyrg") == 0);
    mt_fail(strcmp(example_config->domain, "hbfhjpockorcupzbsqrammuwe") == 0);
    mt_fail(example_config->daemonize == 1);
    mt_fail(strcmp(example_config->user, "qrjtgwdcwkifjonfwolp") == 0);
    mt_fail(strcmp(example_config->group, "tyidtsvsfexomsdznh") == 0);
    mt_fail(strcmp(example_config->pid_file, "psnlixceddgvzzzhkswlfficshvp") == 0);
    mt_fail(strcmp(example_config->list_file, "cnkqizxxmogjauajtenuvpqpmaiysednjkqiis") == 0);
#if HAVE_LST
    mt_fail(example_config->list_type == 0);
#endif
    mt_fail(strcmp(example_config->list_ip_bind, "ogrnjwpjcqr") == 0);
    mt_fail(strcmp(example_config->log_query, "pbztwvmzxioatpfiwylzrciskijindyrkv") == 0);
    mt_fail(strcmp(example_config->log_path, "xzieuhvotfld") == 0);
    mt_fail(example_config->log_level == 2);
    mt_fail(example_config->log_frotate_number == 0);
    mt_fail(example_config->log_colors == 0);
    mt_fail(example_config->log_fsync_level == 1);
    mt_fail(example_config->log_frotate_size == 10485760);
    mt_fail(example_config->log_fsync_every == 4096);
    mt_fail(example_config->log_ts == 2);
    mt_fail(example_config->log_ts_tm == 2);
    mt_fail(example_config->log_ts_tm_fract == 1);
    mt_fail(example_config->log_finfo == 0);
    mt_fail(example_config->log_funcinfo == 0);
    mt_fail(example_config->log_output == 0);
    mt_fail(strcmp(example_config->log_prefix, "termsend: ") == 0);
    mt_fail(example_config->log_level_flag == 6);
    mt_fail(example_config->max_filesize == 20918);
    mt_fail(example_config->max_connections == 30594);
    mt_fail(example_config->max_timeout == 20403);
    mt_fail(example_config->max_timed_timeout == 31051);
    mt_fail(example_config->port_plain == 27133);
    mt_fail(example_config->port_plain_timed == 64271);
#if HAVE_SSL
    mt_fail(example_config->port_ssl == 57554);
    mt_fail(example_config->port_ssl_timed == 34394);
    mt_fail(strcmp(example_config->ssl_key_file, "ffqyydsaripufotkfefjvbwrpernelttojmiahri") == 0);
    mt_fail(strcmp(example_config->ssl_pass_file, "xgukkhszrijklea") == 0);
    mt_fail(strcmp(example_config->ssl_cert_file, "aaqvxxgzgouq") == 0);
#endif


    mt_fok(memcmp(&config, ir_stats_config, sizeof(config)));
}

#endif


/* ==========================================================================
   ========================================================================== */


#if IR_STATS_ENABLE_GETOPT_LONG

static void config_long_opts(void)
{
    char *argv[] =
    {
        "ir-stats",
        "--output-dir=aysrlzggpdlvaqxseyhej",
        "--domain=qvvroxjqpsnqyngkyleaelgfutnndf",
        "--daemonize",
        "--user=rotksjuqmbenzaiifsoilu",
        "--group=iqfmtofsho",
        "--pid-file=dzwcgqrjvygdxorqheo",
        "--list-file=dxakiiumzaxmicuqh",
#if HAVE_LST
        "--list-type=none",
#endif
        "--list-ip-bind=aorvvkoljqujbtlmqfzw",
        "--log-query=expfqitmtdnyesauejpxot",
        "--log-path=qyviydiannktpujfdhbvhingthahhpixkpiqx",
        "--log-level=crit",
        "--log-frotate-number=9574",
        "--log-colors",
        "--log-fsync-level=1",
        "--log-frotate-size=27952",
        "--log-fsync-every=22995",
        "--log-ts=short",
        "--log-ts-tm=monotonic",
        "--log-ts-tm-fract=off",
        "--log-finfo",
        "--log-funcinfo",
        "--log-output=stdout",
        "--log-prefix=tosvoqiprcajxykzrptryocayhg",
        "--log-level-flag=error",
        "--max-filesize=8211",
        "--max-connections=8587",
        "--max-timeout=18739",
        "--max-timed-timeout=520",
        "--port-plain=7565",
        "--port-plain-timed=20425",
#if HAVE_SSL
        "--port-ssl=29242",
        "--port-ssl-timed=64916",
        "--ssl-key-file=sqjohsxrqxsxhsxqrbawsxatovuyuctagjalro",
        "--ssl-pass-file=aacbumhjylyncsumpfxcooc",
        "--ssl-cert-file=asmkzjknzzvbgohrrzyqrfiwd"
#endif

    };
    int argc = sizeof(argv) / sizeof(const char *);
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    ir_stats_config_init(argc, argv);

    /* seed: 2021-03-04 21:42:58.760599 */
    strcpy(config.output_dir, "aysrlzggpdlvaqxseyhej");
    strcpy(config.domain, "qvvroxjqpsnqyngkyleaelgfutnndf");
    config.daemonize = 1;
    strcpy(config.user, "rotksjuqmbenzaiifsoilu");
    strcpy(config.group, "iqfmtofsho");
    strcpy(config.pid_file, "dzwcgqrjvygdxorqheo");
    strcpy(config.list_file, "dxakiiumzaxmicuqh");
#if HAVE_LST
    config.list_type = 1;
#endif
    strcpy(config.list_ip_bind, "aorvvkoljqujbtlmqfzw");
    strcpy(config.log_query, "expfqitmtdnyesauejpxot");
    strcpy(config.log_path, "qyviydiannktpujfdhbvhingthahhpixkpiqx");
    config.log_level = 2;
    config.log_frotate_number = 9574;
    config.log_colors = 1;
    config.log_fsync_level = 1;
    config.log_frotate_size = 27952;
    config.log_fsync_every = 22995;
    config.log_ts = 1;
    config.log_ts_tm = 3;
    config.log_ts_tm_fract = 0;
    config.log_finfo = 1;
    config.log_funcinfo = 1;
    config.log_output = 1;
    strcpy(config.log_prefix, "tosvoqiprcajxykzrptryocayhg");
    config.log_level_flag = 3;
    config.max_filesize = 8211;
    config.max_connections = 8587;
    config.max_timeout = 18739;
    config.max_timed_timeout = 520;
    config.port_plain = 7565;
    config.port_plain_timed = 20425;
#if HAVE_SSL
    config.port_ssl = 29242;
    config.port_ssl_timed = 64916;
    strcpy(config.ssl_key_file, "sqjohsxrqxsxhsxqrbawsxatovuyuctagjalro");
    strcpy(config.ssl_pass_file, "aacbumhjylyncsumpfxcooc");
    strcpy(config.ssl_cert_file, "asmkzjknzzvbgohrrzyqrfiwd");
#endif


    mt_fail(strcmp(example_config->output_dir, "aysrlzggpdlvaqxseyhej") == 0);
    mt_fail(strcmp(example_config->domain, "qvvroxjqpsnqyngkyleaelgfutnndf") == 0);
    mt_fail(example_config->daemonize == 1);
    mt_fail(strcmp(example_config->user, "rotksjuqmbenzaiifsoilu") == 0);
    mt_fail(strcmp(example_config->group, "iqfmtofsho") == 0);
    mt_fail(strcmp(example_config->pid_file, "dzwcgqrjvygdxorqheo") == 0);
    mt_fail(strcmp(example_config->list_file, "dxakiiumzaxmicuqh") == 0);
#if HAVE_LST
    mt_fail(example_config->list_type == 1);
#endif
    mt_fail(strcmp(example_config->list_ip_bind, "aorvvkoljqujbtlmqfzw") == 0);
    mt_fail(strcmp(example_config->log_query, "expfqitmtdnyesauejpxot") == 0);
    mt_fail(strcmp(example_config->log_path, "qyviydiannktpujfdhbvhingthahhpixkpiqx") == 0);
    mt_fail(example_config->log_level == 2);
    mt_fail(example_config->log_frotate_number == 9574);
    mt_fail(example_config->log_colors == 1);
    mt_fail(example_config->log_fsync_level == 1);
    mt_fail(example_config->log_frotate_size == 27952);
    mt_fail(example_config->log_fsync_every == 22995);
    mt_fail(example_config->log_ts == 1);
    mt_fail(example_config->log_ts_tm == 3);
    mt_fail(example_config->log_ts_tm_fract == 0);
    mt_fail(example_config->log_finfo == 1);
    mt_fail(example_config->log_funcinfo == 1);
    mt_fail(example_config->log_output == 1);
    mt_fail(strcmp(example_config->log_prefix, "tosvoqiprcajxykzrptryocayhg") == 0);
    mt_fail(example_config->log_level_flag == 3);
    mt_fail(example_config->max_filesize == 8211);
    mt_fail(example_config->max_connections == 8587);
    mt_fail(example_config->max_timeout == 18739);
    mt_fail(example_config->max_timed_timeout == 520);
    mt_fail(example_config->port_plain == 7565);
    mt_fail(example_config->port_plain_timed == 20425);
#if HAVE_SSL
    mt_fail(example_config->port_ssl == 29242);
    mt_fail(example_config->port_ssl_timed == 64916);
    mt_fail(strcmp(example_config->ssl_key_file, "sqjohsxrqxsxhsxqrbawsxatovuyuctagjalro") == 0);
    mt_fail(strcmp(example_config->ssl_pass_file, "aacbumhjylyncsumpfxcooc") == 0);
    mt_fail(strcmp(example_config->ssl_cert_file, "asmkzjknzzvbgohrrzyqrfiwd") == 0);
#endif


    mt_fok(memcmp(&config, ir_stats_config, sizeof(config)));
}

#endif


/* ==========================================================================
   ========================================================================== */


#if IR_STATS_ENABLE_INI

static void config_from_file(void)
{
    char *argv[] =
    {
        "ir-stats",
        "-ctest.ini"
    };
    int argc = sizeof(argv) / sizeof(const char *);
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


    ir_stats_config_init(argc, argv);

    /* seed: 2021-03-04 21:42:58.770695 */
    strcpy(config.output_dir, "lkxbzdrvsvpuhkyqpnoadzvbujk");
    strcpy(config.domain, "uwltqnurivwjyytw");
    config.daemonize = 1;
    strcpy(config.user, "qszoprmzofhnyjyaac");
    strcpy(config.group, "aexxnyrdtqwiynbddlgwspfoocwqtdw");
    strcpy(config.pid_file, "frlzhcmxkgsmorggtfouwtpnhu");
    strcpy(config.list_file, "tmerzuyfnfrnpctvimfsh");
#if HAVE_LST
    config.list_type = 1;
#endif
    strcpy(config.list_ip_bind, "uzypxobcp");
    strcpy(config.log_query, "dgxwgsagkuizzjcljod");
    strcpy(config.log_path, "wmgczhfkfabhvdmazfpnawepgabnufwbvnwtse");
    config.log_level = 4;
    config.log_frotate_number = 7598;
    config.log_colors = 1;
    config.log_fsync_level = 2;
    config.log_frotate_size = 11680;
    config.log_fsync_every = 20533;
    config.log_ts = 1;
    config.log_ts_tm = 0;
    config.log_ts_tm_fract = 0;
    config.log_finfo = 1;
    config.log_funcinfo = 1;
    config.log_output = 0;
    strcpy(config.log_prefix, "uahyaur");
    config.log_level_flag = 4;
    config.max_filesize = 2750;
    config.max_connections = 29486;
    config.max_timeout = 2238;
    config.max_timed_timeout = 19134;
    config.port_plain = 17907;
    config.port_plain_timed = 1003;
#if HAVE_SSL
    config.port_ssl = 32546;
    config.port_ssl_timed = 15931;
    strcpy(config.ssl_key_file, "oijgphnfq");
    strcpy(config.ssl_pass_file, "boxagdculabnihtepqnkfmvf");
    strcpy(config.ssl_cert_file, "fwxfoxnjiiugep");
#endif


    mt_fail(strcmp(example_config->output_dir, "lkxbzdrvsvpuhkyqpnoadzvbujk") == 0);
    mt_fail(strcmp(example_config->domain, "uwltqnurivwjyytw") == 0);
    mt_fail(example_config->daemonize == 1);
    mt_fail(strcmp(example_config->user, "qszoprmzofhnyjyaac") == 0);
    mt_fail(strcmp(example_config->group, "aexxnyrdtqwiynbddlgwspfoocwqtdw") == 0);
    mt_fail(strcmp(example_config->pid_file, "frlzhcmxkgsmorggtfouwtpnhu") == 0);
    mt_fail(strcmp(example_config->list_file, "tmerzuyfnfrnpctvimfsh") == 0);
#if HAVE_LST
    mt_fail(example_config->list_type == 1);
#endif
    mt_fail(strcmp(example_config->list_ip_bind, "uzypxobcp") == 0);
    mt_fail(strcmp(example_config->log_query, "dgxwgsagkuizzjcljod") == 0);
    mt_fail(strcmp(example_config->log_path, "wmgczhfkfabhvdmazfpnawepgabnufwbvnwtse") == 0);
    mt_fail(example_config->log_level == 4);
    mt_fail(example_config->log_frotate_number == 7598);
    mt_fail(example_config->log_colors == 1);
    mt_fail(example_config->log_fsync_level == 2);
    mt_fail(example_config->log_frotate_size == 11680);
    mt_fail(example_config->log_fsync_every == 20533);
    mt_fail(example_config->log_ts == 1);
    mt_fail(example_config->log_ts_tm == 0);
    mt_fail(example_config->log_ts_tm_fract == 0);
    mt_fail(example_config->log_finfo == 1);
    mt_fail(example_config->log_funcinfo == 1);
    mt_fail(example_config->log_output == 0);
    mt_fail(strcmp(example_config->log_prefix, "uahyaur") == 0);
    mt_fail(example_config->log_level_flag == 4);
    mt_fail(example_config->max_filesize == 2750);
    mt_fail(example_config->max_connections == 29486);
    mt_fail(example_config->max_timeout == 2238);
    mt_fail(example_config->max_timed_timeout == 19134);
    mt_fail(example_config->port_plain == 17907);
    mt_fail(example_config->port_plain_timed == 1003);
#if HAVE_SSL
    mt_fail(example_config->port_ssl == 32546);
    mt_fail(example_config->port_ssl_timed == 15931);
    mt_fail(strcmp(example_config->ssl_key_file, "oijgphnfq") == 0);
    mt_fail(strcmp(example_config->ssl_pass_file, "boxagdculabnihtepqnkfmvf") == 0);
    mt_fail(strcmp(example_config->ssl_cert_file, "fwxfoxnjiiugep") == 0);
#endif


    mt_fok(memcmp(&config, ir_stats_config, sizeof(config)));

}

#endif
/* ==========================================================================
   ========================================================================== */


static void config_print_help(void)
{
    int    argc = 2;
    char  *argv[] = { "ir-stats", "-h", NULL };
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


    mt_fail(ir_stats_config_init(argc, argv) == -2);
}


/* ==========================================================================
   ========================================================================== */


static void config_print_version(void)
{
    int    argc = 2;
    char  *argv[] = { "ir-stats", "-v", NULL };
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


    mt_fail(ir_stats_config_init(argc, argv) == -3);
}


/* ==========================================================================
             __               __
            / /_ ___   _____ / /_   ____ _ _____ ____   __  __ ____
           / __// _ \ / ___// __/  / __ `// ___// __ \ / / / // __ \
          / /_ /  __/(__  )/ /_   / /_/ // /   / /_/ // /_/ // /_/ /
          \__/ \___//____/ \__/   \__, //_/    \____/ \__,_// .___/
                                 /____/                    /_/
   ========================================================================== */


void config_run_tests()
{
    mt_prepare_test = &test_prepare;

    mt_run(config_all_default);
#if IR_STATS_ENABLE_GETOPT || IR_STATS_ENABLE_GETOPT_LONG
    mt_run(config_short_opts);
#endif
#if IR_STATS_ENABLE_GETOPT_LONG
    mt_run(config_long_opts);
#endif
#if IR_STATS_ENABLE_INI
    mt_run(config_from_file);
#endif
    mt_run(config_print_help);
    mt_run(config_print_version);
}
