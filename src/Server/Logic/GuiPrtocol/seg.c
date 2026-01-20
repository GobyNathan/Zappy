/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** pic.c
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include <stdio.h>

void send_seg_all(char *team, clients_management_t *manage)
{
    char msg[64] = {0};

    snprintf(msg, sizeof(msg), "seg %s\n", team);
    send_msg_graphics(manage, msg);
}
