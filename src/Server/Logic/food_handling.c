/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** food_handling.c
*/

#include "Server/gui_protocol.h"
#include "Server/server_info.h"
#include "Server/server.h"
#include <stddef.h>

void decrease_food_players(server_info_t *info,
    clients_management_t *client_manage)
{
    for (int i = 0; info->individuals[i] != NULL; ++i){
        if (info->individuals[i]->is_alive)
            info->individuals[i]->food_lvl--;
        if (info->individuals[i]->is_alive &&
            info->individuals[i]->food_lvl >= 0)
            send_pin_all(info->individuals[i], client_manage);
        if (info->individuals[i]->is_alive &&
            info->individuals[i]->food_lvl < 0)
            info->individuals[i]->is_alive = false;
    }
    remove_dead_players(client_manage);
}
