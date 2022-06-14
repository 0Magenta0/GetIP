#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <json-c/json.h>

#include "sock_utils.h"

static int sock;
static struct sockaddr_in addr;
static char buf [BUF_SIZE];
static struct json_object *parsed_json;
static struct json_object *certain_json_obj;

void socket_init (void) {
    if ((sock = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
        perror ("[\033[1;31m-\033[0m] Socket init error");
        exit (2);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr ("208.95.112.1");
    addr.sin_port = htons (80);

    if (connect (sock, (struct sockaddr *) &addr, sizeof (addr)) == -1) {
        perror ("[\033[1;31m-\033[0m] Connection error");
        exit (3);
    }
}

void release_request (void) {
    struct json_tokener *tokener_ex;
    char *content;

    if (!external_ip) {
        sprintf (buf, "GET /json/?fields=%d HTTP/1.1\r\nHost: ip-api.com\r\nConnection: close\r\n\r\n", api_bitset_word);
    } else {
        sprintf (buf, "GET /json/%.255s?fields=%d HTTP/1.1\r\nHost: ip-api.com\r\nConnection: close\r\n\r\n", external_ip, api_bitset_word);
    }

    if (send (sock, buf, strlen (buf), 0) <= 0) {
        perror ("[\033[1;31m-\033[0m] Request error");
        exit (4);
    } if (recv (sock, buf, BUF_SIZE, 0) <= 0) {
        perror ("[\033[1;31m-\033[0m] Response error");
        exit (5);
    } if (!strstr (buf, "HTTP/1.1 200 OK")) {
        fputs ("[\033[1;31m-\033[0m] HTTP error: Response code is not \"200 OK\"\n", stderr);
        exit (6);
    }

    close (sock);

    /* Отбрасывание HTTP Header-ов от Json */
    if (!(content = strstr (buf, "\r\n\r\n")))  {
        fputs ("[\033[1;31m-\033[0m] HTTP error: Cannot find start JSON position\n", stderr);
        exit (9);
    } content += 4; /* Перемещение указателя за \r\n\r\n */

    /* Парсинг JSON */
    tokener_ex = json_tokener_new ();
    if (!(parsed_json = json_tokener_parse_ex (tokener_ex, content, strlen (content)))) {
        fprintf (stderr, "[\033[1;31m-\033[0m] JSON error: %d\n[\033[1;33m*\033[0m] JSON buffer:\n%s\n", json_tokener_get_error (tokener_ex), content);
        exit (8);
    } json_tokener_free (tokener_ex);

    json_object_object_get_ex (parsed_json, "status", &certain_json_obj);
    if (strcmp (json_object_get_string (certain_json_obj), "success")) {
        printf ("[\033[1;31m-\033[0m] API error: Response status is \"%s\"\n", json_object_get_string (certain_json_obj));
        json_object_object_get_ex (parsed_json, "message", &certain_json_obj);
        printf ("[\033[1;31m-\033[0m] API message: %s\n", json_object_get_string (certain_json_obj));
        exit (7);
    }
}

void read_response (void) {
    int i;

    for (i = 0; param_objs [i].output_str; ++i) {
        if (param_objs [i].toggle) {
            json_object_object_get_ex (parsed_json, param_objs [i].json_obj_n, &certain_json_obj);
            if (!json_object_is_type (certain_json_obj, json_type_null) && (json_object_get_string_len (certain_json_obj) || json_object_is_type (certain_json_obj, json_type_boolean))) {
                printf ("[\033[1;32m+\033[0m] %s: %s\n", param_objs [i].output_str, json_object_get_string (certain_json_obj));
            } else {
                printf ("[\033[1;31m-\033[0m] %s: \033[1;31m%s\033[0m\n", param_objs [i].output_str, ERR_GETIP_STR);
            }
        }
    }
}

