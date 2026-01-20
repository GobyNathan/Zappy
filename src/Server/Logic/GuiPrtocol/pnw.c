/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pnw.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include "Server/utilities.h"
#include "Server/messages.h"
#include <stdio.h>

void send_pnw(individual_t *individual, client_t *client)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_PNW, individual->player_id,
        individual->pos_x, individual->pos_y, individual->look_at + 1,
        individual->main_lvl, individual->team);
    send_msg_client(client, msg);
}

void send_pnw_all(individual_t *individual,
    clients_management_t *client_manage)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_PNW, individual->player_id,
        individual->pos_x, individual->pos_y, individual->look_at + 1,
        individual->main_lvl, individual->team);
    send_msg_graphics(client_manage, msg);
}
