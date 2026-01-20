/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** sgt.c
*/

#include "Server/gui_protocol.h"
#include "Server/utilities.h"
#include "Server/messages.h"
#include <stdio.h>

void sgt_command(server_info_t *info, client_t *client)
{
    char response[1024] = {0};

    snprintf(response, sizeof(response), MSG_SGT, info->freq);
    send_msg_client(client, response);
}
