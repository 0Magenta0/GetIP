#include "getip_utils.h"

/* Размер буфера для обмена данными с сервером */
#define BUF_SIZE BUFSIZ /* BUFSIZ определён в stdio.h */

#define ERR_GETIP_STR "Error" /* Строка-заменитель для невалидного ответа API */

/* Битовые флаги для API -=- Docs: https://ip-api.com/docs/api:json */
enum api_bitset {
    en_ip_bit = 8192,
    en_org_bit = 1024,
    en_hostname_bit = 4096,
    en_as_bit = 2048,
    en_asname_bit = 4194304,
    en_isp_bit = 512,
    en_continent_bit = 1048576,
    en_country_bit = 1,
    en_region_bit = 8,
    en_city_bit = 16,
    en_timezone_bit = 256,
    en_hosting_bit = 16777216,
    en_proxy_bit = 131072,
    en_mobile_bit = 65536
};

/* Enumerator параметров API */
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

/* Объект параметра API */
struct param_obj {
    char* output_str;
    char* json_obj_n;
    char toggle;
};

/* Заменители PlaceHoder-ов в выоде информации и обращении к Json объектам */
extern struct param_obj param_objs [14];

void socket_init (void); /* Создание сокета и инициализаця адреса */
void release_request (void); /* Запрос на сервер */
void read_response (void); /* Вывод информации об IP */

