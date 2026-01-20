/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pfk.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include "Server/messages.h"
#include <stdio.h>

void send_pfk_all(individual_t *individual,
    clients_management_t *client_manage)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_PFK, individual->player_id);
    send_msg_graphics(client_manage, msg);
}
