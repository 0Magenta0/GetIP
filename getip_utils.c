#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "getip_utils.h"
#include "sock_utils.h"

#define _GNU_SOURCE

char* external_ip;
unsigned int api_bitset_word = 49152 /* 2 битовых флага включены по умолчанию */;

struct param_obj param_objs [14] = { { "IP", "query", 0 },
                                     { "ORG", "org", 0 },
                                     { "Hostname", "reverse", 0 },
                                     { "AS", "as", 0 },
                                     { "AS Name", "asname", 0 },
                                     { "ISP", "isp", 0 },
                                     { "Continent", "continent", 0 },
                                     { "Country", "country", 0 },
                                     { "Region", "regionName", 0 },
                                     { "City", "city", 0 },
                                     { "Time Zone", "timezone", 0 },
                                     { "isHosting", "hosting", 0 },
                                     { "isProxy", "proxy", 0 },
                                     { "isMobile", "mobile", 0 } };

void print_help (int exit_code) {
    puts ("Usage: getip <args>\n\n"
          "  -h\t\tPrint this message\n"
          "  -e <str>\tUse another's IP\n"
          "  -4\t\tPrint IP parameter\n"
          "  -o\t\tPrint ORG parameter\n"
          "  -n\t\tPrint Hostname parameter\n"
          "  -a\t\tPrint AS parameter\n"
          "  -A\t\tPrint AS Name parameter\n"
          "  -i\t\tPrint ISP parameter\n"
          "  -C\t\tPrint Continent parameter\n"
          "  -c\t\tPrint Country parameter\n"
          "  -r\t\tPrint Region parameter\n"
          "  -t\t\tPrint City parameter\n"
          "  -z\t\tPrint TimeZone parameter\n"
          "  -H\t\tPrint Hosting parameter\n"
          "  -p\t\tPrint Proxy parameter\n"
          "  -m\t\tPrint Mobile parameter\n\n"

          "Version: 1.1.5\n"
          "Author: _Magenta_\n");
    exit (exit_code);
}

void parameter_handler (int ac, char** av) {
    opterr = 0;
    int arg;
    for (; (arg = getopt (ac, av, "4onaAiCcrtzHpmhe:")) != -1;) {
        switch (arg) {
            case 'h':
                print_help (0);
                break;

            case 'e':
                external_ip = optarg;
                break;

            case '4':
                param_objs [en_ip].toggle = 1;
                api_bitset_word |= en_ip_bit;
                break;

            case 'o':
                param_objs [en_org].toggle = 1;
                api_bitset_word |= en_org_bit;
                break;

            case 'n':
                param_objs [en_hostname].toggle = 1;
                api_bitset_word |= en_hostname_bit;
                break;

            case 'a':
                param_objs [en_as].toggle = 1;
                api_bitset_word |= en_as_bit;
                break;

            case 'A':
                param_objs [en_asname].toggle = 1;
                api_bitset_word |= en_asname_bit;
                break;

            case 'i':
                param_objs [en_isp].toggle = 1;
                api_bitset_word |= en_isp_bit;
                break;

            case 'C':
                param_objs [en_continent].toggle = 1;
                api_bitset_word |= en_continent_bit;
                break;

            case 'c':
                param_objs [en_country].toggle = 1;
                api_bitset_word |= en_country_bit;
                break;

            case 'r':
                param_objs [en_region].toggle = 1;
                api_bitset_word |= en_region_bit;
                break;

            case 't':
                param_objs [en_city].toggle = 1;
                api_bitset_word |= en_city_bit;
                break;

            case 'z':
                param_objs [en_timezone].toggle = 1;
                api_bitset_word |= en_timezone_bit;
                break;

            case 'H':
                param_objs [en_hosting].toggle = 1;
                api_bitset_word |= en_hosting_bit;
                break;

            case 'p':
                param_objs [en_proxy].toggle = 1;
                api_bitset_word |= en_proxy_bit;
                break;

            case 'm':
                param_objs [en_mobile].toggle = 1;
                api_bitset_word |= en_mobile_bit;
                break;

            default:
                print_help (1);
                break;
        }
    }

    if (!check_toggle ()) {
        api_bitset_word = 22282009 /* Все битовые флаги для API */;
        int i;
        for (i = 0; param_objs [i].output_str; ++i)
            param_objs [i].toggle = 1;
    }
}

char check_toggle (void) {
    int i;
    for (i = 0; param_objs [i].output_str; ++i)
        if (param_objs [i].toggle)
            return 1;

    return 0;
}

void exec_requests (void) {
    socket_init ();
    release_request ();
    read_response ();
}

