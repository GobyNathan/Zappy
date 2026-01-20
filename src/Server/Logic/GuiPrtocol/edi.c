/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pic.c
*/

#include "Server/messages.h"
#include "Server/server_info.h"
#include "Server/server.h"
#include <stdio.h>
#include "Server/gui_protocol.h"
#include "Server/server.h"
#include "Server/utilities.h"

void send_edi(individual_t *egg, client_t *client)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_EDI, egg->player_id);
    send_msg_client(client, msg);
}

void send_edi_all(individual_t *egg,
    clients_management_t *client_manage)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_EDI, egg->player_id);
    send_msg_graphics(client_manage, msg);
}
