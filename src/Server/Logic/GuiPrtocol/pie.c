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

void send_pie_all(int posX, int posY, bool success,
    clients_management_t *manage)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), "pie %d %d %d\n", posX,
        posY, success);
    send_msg_graphics(manage, msg);
}
