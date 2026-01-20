/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** msgs_elevation.c
*/

#include "Server/server_info.h"
#include "Server/elevation_utilities.h"
#include "Server/utilities.h"
#include "Server/gui_protocol.h"
#include "Server/ia_messages.h"
#include <stddef.h>
#include <stdio.h>


void msg_elevation_begin(int *group, int group_size,
    clients_management_t *manage)
{
    individual_t *tmp = NULL;

    for (int i = 0; i < manage->len_clients; ++i){
        tmp = manage->clients[i].individual;
        if (tmp == NULL)
            continue;
        if (tmp->player_id == group[0])
            send_pic_all(group, group_size, tmp, manage);
        if (in_group(group, group_size, tmp->player_id))
            send_msg_client(&manage->clients[i], MSG_INCANTATION);
    }
}

void msg_elevation_end(client_t *client)
{
    char msg[1024] = {0};

    snprintf(msg, 1024, MSG_CURRENT_LEVEL, client->individual->main_lvl);
    send_msg_client(client, msg);
}
