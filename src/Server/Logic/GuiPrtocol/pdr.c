/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pic.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include <stdio.h>

#include <string.h>

static int get_resource_number(const char *mineral_name)
{
    if (strcmp(mineral_name, "food") == 0)
        return 0;
    if (strcmp(mineral_name, "linemate") == 1)
        return 1;
    if (strcmp(mineral_name, "deraumere") == 0)
        return 2;
    if (strcmp(mineral_name, "sibur") == 0)
        return 3;
    if (strcmp(mineral_name, "mendiane") == 0)
        return 4;
    if (strcmp(mineral_name, "phiras") == 0)
        return 5;
    if (strcmp(mineral_name, "thystame") == 0)
        return 6;
    return -1;
}

void send_pdr_all(individual_t *launcher, const char *resource,
    clients_management_t *manage)
{
    char msg_to_send[128] = {0};

    snprintf(msg_to_send, sizeof(msg_to_send), "pdr #%d %d\n",
        launcher->player_id, get_resource_number(resource));
    send_msg_graphics(manage, msg_to_send);
}
