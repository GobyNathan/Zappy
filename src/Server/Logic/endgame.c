/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** endgame.c
*/

#include <stdbool.h>
#include "Server/server_info.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "Server/utilities.h"
#include "Server/gui_protocol.h"

static char *get_winning_team(char **teams, individual_t **players)
{
    int nb_max_lvl = 0;

    for (int i = 0; teams[i] != NULL; ++i){
        for (int j = 0; players[j] != NULL; ++j){
            nb_max_lvl += strcmp(teams[i], players[j]->team) == 0 &&
                players[j]->main_lvl == MAX_LEVEL ? 1 : 0;
        }
        if (nb_max_lvl >= 6)
            return teams[i];
        nb_max_lvl = 0;
    }
    return NULL;
}

bool is_endgame(server_info_t *info)
{
    if (get_winning_team(info->names, info->individuals) != NULL)
        return true;
    return false;
}

void endgame(server_info_t *info, clients_management_t *manage)
{
    char *w_team = get_winning_team(info->names, info->individuals);

    send_seg_all(w_team, manage);
    for (int i = 0; i < manage->len_clients; ++i){
        remove_client(manage, i);
    }
    exit(0);
}
