#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include "sock_utils.h"

void socketInit () {
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

void releaseRequest () {
    if (!bExternalIP) {
        strcpy (buf, "GET /json/?fields=22806301 HTTP/1.1\r\nHost: demo.ip-api.com\r\nConnection: close\r\n\r\n");
    } else {
        sprintf (buf, "GET /json/%s?fields=22806301 HTTP/1.1\r\nHost: demo.ip-api.com\r\nConnection: close\r\n\r\n", externalIP);
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
    
    parsedJson = json_tokener_parse (buf);
    json_object_object_get_ex (parsedJson, "status", &certainJsonObj);
    if (strcmp (json_object_get_string (certainJsonObj), "success")) {
        printf ("[\033[1;31m-\033[0m] API error: Response status is \"%s\"\n", json_object_get_string (certainJsonObj));
        json_object_object_get_ex (parsedJson, "message", &certainJsonObj);
        printf ("[\033[1;31m-\033[0m] API message: %s\n", json_object_get_string (certainJsonObj));
        exit (7);
    }

    // Вывод полученной информации
    json_object_object_get_ex (parsedJson, paramStrs [0][1], &certainJsonObj);
    if (!json_object_is_type (certainJsonObj, json_type_null) && json_object_get_string_len (certainJsonObj)) {
        printf ("[\033[1;32m+\033[0m] %s: %s\n", paramStrs [0][0], json_object_get_string (certainJsonObj));
    } else {
        printf ("[\033[1;31m-\033[0m] %s: \033[1;31m%s\033[0m\n", paramStrs [0][0], ERR_GETIP_STR);
    } if (!only4) {
        for (int i = 1; paramStrs [i][0]; ++i) {
            json_object_object_get_ex (parsedJson, paramStrs [i][1], &certainJsonObj);
            if (!json_object_is_type (certainJsonObj, json_type_null) && (json_object_get_string_len (certainJsonObj) || json_object_is_type (certainJsonObj, json_type_boolean))) {
                printf ("[\033[1;32m+\033[0m] %s: %s\n", paramStrs [i][0], json_object_get_string (certainJsonObj));
            } else {
                printf ("[\033[1;31m-\033[0m] %s: \033[1;31m%s\033[0m\n", paramStrs [i][0], ERR_GETIP_STR); 
            }
        }   
    }
}

