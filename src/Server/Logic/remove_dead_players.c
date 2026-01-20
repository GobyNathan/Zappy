/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** remove_dead_players.c
*/

#include "Server/server_info.h"
#include "Server/utilities.h"
#include <stddef.h>
#include <stdio.h>

void remove_dead_players(clients_management_t *client_manage)
{
    client_t *clients = client_manage->clients;

    for (int i = 0; i < client_manage->len_clients; ++i){
        if (clients[i].individual != NULL && !clients[i].individual->is_alive){
            send_msg_client(&clients[i], "dead\n");
            remove_client(client_manage, i);
        }
    }
}
