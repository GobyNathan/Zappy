/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pic.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include <stdio.h>

void send_pbc_all(individual_t *launcher, const char *content,
    clients_management_t *manage)
{
    char msg_to_send[128] = {0};

    snprintf(msg_to_send, sizeof(msg_to_send), "pbc #%d %s\n",
        launcher->player_id, content);
    send_msg_graphics(manage, msg_to_send);
}
