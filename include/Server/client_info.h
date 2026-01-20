/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/


#ifndef CLIENT_INFO_H
    #define CLIENT_INFO_H
    #include <stdbool.h>
    #include "individual_info.h"
    #define CLIENT_BUFFER_SIZE 1024

typedef struct client_a {
    int sockfd;
    int id;
    char *team;
    individual_t *individual;
    char *buffer;
} client_t;

#endif /* CLIENT_INFO_H */
