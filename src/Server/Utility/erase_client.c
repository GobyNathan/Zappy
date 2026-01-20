/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** array_utilities.c
*/

#include "Server/server_info.h"
#include <stdio.h>
#include <stdlib.h>

void erase_client(clients_management_t *client_manage, int index_erase)
{
    int len = client_manage->len_clients;
    client_t *new_clients = NULL;

    for (int i = index_erase; i < len - 1; ++i)
        client_manage->clients[i] = client_manage->clients[i + 1];
    client_manage->len_clients--;
    if (client_manage->len_clients == 0) {
        free(client_manage->clients);
        client_manage->clients = NULL;
        return;
    }
    new_clients = realloc(client_manage->clients,
        sizeof(client_t) * client_manage->len_clients);
    if (new_clients == NULL) {
        perror("realloc failed");
        return;
    }
    client_manage->clients = new_clients;
}
