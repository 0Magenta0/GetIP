#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "getip_utils.h"

#define BUF_SIZE 1024

static char* paramStrs [][2] = { { "IP", "query" },
                                 { "ORG", "org" },
                                 { "Hostname", "reverse" },
                                 { "AS", "as" },
                                 { "AS Name", "asname" },
                                 { "ISP", "isp" },
                                 { "Континент", "continent" },
                                 { "Страна", "country" },
                                 { "Регион", "regionName" },
                                 { "Город", "city" },
                                 { "Часовой Пояс", "timezone" },
                                 { "Хостинг", "hosting" },
                                 { "Прокси", "proxy" },
                                 { "Мобила", "mobile" } };


static int sock;
static struct sockaddr_in addr;
static char buf [BUF_SIZE];
static struct json_object* parsedJson;
static struct json_object* certainJsonObj;

void socketInit ();
void releaseRequest ();

