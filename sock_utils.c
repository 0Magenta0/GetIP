#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include "sock_utils.h"

void socket_init () {
    if ((sock = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
        perror ("[\033[1;31m-\033[0m] Socket init error");
        exit (2);
    }

    addr.sin_addr.s_addr = inet_addr ("208.95.112.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons (80);

    if (connect (sock, (struct sockaddr*) &addr, sizeof (addr)) == -1) {
        perror ("[\033[1;31m-\033[0m] Connection error");
        exit (3);
    }
}

void release_request () {
    if (!b_external_ip) {
        strcpy (buf, "GET /json/?fields=22806301 HTTP/1.1\r\nHost: demo.ip-api.com\r\nConnection: close\r\n\r\n");
    } else {
        sprintf (buf, "GET /json/%s?fields=22806301 HTTP/1.1\r\nHost: demo.ip-api.com\r\nConnection: close\r\n\r\n", external_ip);
    }

    if (send (sock, buf, strlen (buf), 0) < 0) {
        perror ("[\033[1;31m-\033[0m] Request error");
        exit (4);
    } if (recv (sock, buf, BUF_SIZE, 0) < 0) {
        perror ("[\033[1;31m-\033[0m] Response error");
        exit (5);
    } if (!strstr (buf, "HTTP/1.1 200 OK")) {
        puts ("[\033[1;31m-\033[0m] HTTP error: Response code is not \"200 OK\"");
        exit (6);
    }

    close (sock);

    // Срезание Header-ов от Json
    for (int i = 0, fnd = strlen (buf) - strlen (strstr (buf, "\r\n\r\n") + 4); buf [fnd + i]; ++i) {
        buf [i] = buf [fnd + i];
    }
    
    parsed_json = json_tokener_parse (buf);
    json_object_object_get_ex (parsed_json, "status", &certain_json_obj);
    if (strcmp (json_object_get_string (certain_json_obj), "success")) {
        printf ("[\033[1;31m-\033[0m] API error: Response status is \"%s\"\n", json_object_get_string (certain_json_obj));
        json_object_object_get_ex (parsed_json, "message", &certain_json_obj);
        printf ("[\033[1;31m-\033[0m] API message: %s\n", json_object_get_string (certain_json_obj));
        exit (7);
    }

    // Вывод полученной информации 
    for (int i = 0; param_objs [i].output_str; ++i) {
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

