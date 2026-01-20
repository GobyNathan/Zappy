/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** mct.c
*/

#include "Server/gui_protocol.h"
#include <stdio.h>

void mct_command(server_info_t *info, client_t *client)
{
    char command[1024] = {0};

    for (int i = 0; i < info->map->size_x; ++i){
        for (int j = 0; j < info->map->size_y; ++j){
            snprintf(command, sizeof(command), "bct %d %d", i, j);
            bct_command(info, client, command);
        }
    }
}
