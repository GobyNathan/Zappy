/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** send_graphics
*/

#include "Server/server_info.h"
#include <string.h>
#include "Server/utilities.h"

void send_msg_graphics(clients_management_t *client_manage, char *msg)
{
    for (int i = 0; i < client_manage->len_clients; ++i){
        if (strcmp(client_manage->clients[i].team, "GRAPHIC") == 0){
            send_msg_client(&client_manage->clients[i], msg);
        }
    }
}
