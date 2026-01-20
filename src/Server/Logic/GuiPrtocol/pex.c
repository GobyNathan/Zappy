/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pic.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include <stdio.h>

void send_pex_all(individual_t *launcher, clients_management_t *manage)
{
    char msg[64] = {0};

    snprintf(msg, sizeof(msg), "pex #%d\n", launcher->player_id);
    send_msg_graphics(manage, msg);
}
