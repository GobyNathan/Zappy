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

void send_enw(individual_t *individual, client_t *client, int forker_id)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_ENW, individual->player_id,
        forker_id, individual->pos_x, individual->pos_y);
    send_msg_client(client, msg);
}

void send_enw_all(individual_t *individual,
    clients_management_t *client_manage, int forker_id)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_ENW, individual->player_id,
        forker_id, individual->pos_x, individual->pos_y);
    send_msg_graphics(client_manage, msg);
}
