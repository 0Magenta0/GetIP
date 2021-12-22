#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "getip_utils.h"

#define BUF_SIZE 1024 // Размер буфера для обмена данными с сервером
#define ERR_GETIP_STR "Ошибка"

enum param_enum {
    en_ip,
    en_org,
    en_hostname,
    en_as,
    en_asname,
    en_isp,
    en_continent,
    en_country,
    en_region,
    en_city,
    en_timezone,
    en_hosting,
    en_proxy,
    en_mobile
};

struct param_obj {
    char* output_str;
    char* json_obj_n;
    _Bool toggle;
};

// Заменители PlaceHoder-ов в выоде информации и обращении к Json объектам
extern struct param_obj param_objs [14];

static int sock;
static struct sockaddr_in addr;
static char buf [BUF_SIZE];
static struct json_object* parsed_json;
static struct json_object* certain_json_obj;

void socket_init (); // Создание сокета и инициализаця адреса
void release_request (); // Запрос на сервер и вывод информации об IP

