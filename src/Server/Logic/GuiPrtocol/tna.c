/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** tna.c
*/

#include "Server/gui_protocol.h"
#include "Server/utilities.h"
#include <stddef.h>
#include <stdio.h>
#include "Server/messages.h"

void tna_command(server_info_t *info, client_t *client)
{
    char response[1024] = {0};

    for (int i = 0; info->names[i] != NULL; ++i){
        snprintf(response, sizeof(response), MSG_TNA, info->names[i]);
        send_msg_client(client, response);
    }
}
