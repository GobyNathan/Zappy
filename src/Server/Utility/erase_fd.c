/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** array_utilities.c
*/

#include "Server/server_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>

void erase_fd(clients_management_t *client_manage, int index_erase)
{
    int current_fds_len = client_manage->len_fds;
    int new_fds_len = client_manage->len_fds - 1;
    int new_size = sizeof(struct pollfd) * new_fds_len;
    struct pollfd *new_fds = NULL;

    for (int i = index_erase; i < current_fds_len - 1; ++i){
        client_manage->fds[i] = client_manage->fds[i + 1];
    }
    new_fds = realloc(client_manage->fds, new_size);
    if (new_fds == NULL){
        perror("Error when realloc fds array");
        return;
    }
    client_manage->fds = new_fds;
    client_manage->len_fds = new_fds_len;
}
