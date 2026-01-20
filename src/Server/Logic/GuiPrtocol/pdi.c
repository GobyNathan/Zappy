/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pdi.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include "Server/utilities.h"
#include "Server/messages.h"
#include <stdio.h>

void send_pdi(individual_t *individual, client_t *client)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_PDI,
        individual->player_id);
    send_msg_client(client, msg);
}

void send_pdi_all(individual_t *individual,
    clients_management_t *client_manage)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_PDI,
        individual->player_id);
    send_msg_graphics(client_manage, msg);
}
