/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pic.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include "Server/utilities.h"
#include "Server/messages.h"
#include <stdio.h>
#include <string.h>

void send_pic_all(int *group, int group_size, individual_t *launcher,
    clients_management_t *manage)
{
    char msg[1024] = {0};
    char player[32] = {0};

    snprintf(msg, sizeof(msg), "pic %d %d %d %d", launcher->pos_x,
        launcher->pos_y, launcher->main_lvl + 1, launcher->player_id);
    for (int i = 1; i < group_size; ++i){
        snprintf(player, sizeof(player), " %d", group[i]);
        strcat(msg, player);
    }
    strcat(msg, "\n");
    send_msg_graphics(manage, msg);
}
