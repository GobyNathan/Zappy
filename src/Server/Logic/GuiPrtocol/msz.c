/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** msz.c
*/

#include "Server/gui_protocol.h"
#include "Server/utilities.h"
#include "Server/messages.h"
#include <stdio.h>

void msz_command(server_info_t *info, client_t *client)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_MSZ, info->map->size_x,
    info->map->size_y);
    send_msg_client(client, msg);
}
