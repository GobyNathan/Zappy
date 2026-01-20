/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** plv.c
*/

#include "Server/gui_protocol.h"
#include "Server/messages.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Server/utilities.h"
#include "Server/server.h"

static int get_player_id(char *command)
{
    char *command_cpy = strdup(command);
    char *id_text = NULL;
    int id = 0;
    char *endptr = NULL;

    strtok(command_cpy, " \t");
    id_text = strtok(NULL, " \t");
    if (id_text == NULL){
        free(command_cpy);
        return -1;
    }
    id = (int) strtol(id_text + 1, &endptr, 10);
    if (*endptr != '\0'){
        free(command_cpy);
        return -1;
    }
    free(command_cpy);
    return id;
}

void plv_command(server_info_t *info, client_t *client, char *command)
{
    char msg[1024] = {0};
    int id = get_player_id(command);
    individual_t *player = NULL;

    for (int i = 0; info->individuals[i] != NULL; ++i){
        if (!info->individuals[i]->is_egg &&
            info->individuals[i]->player_id == id)
            player = info->individuals[i];
    }
    if (player == NULL){
        send_msg_client(client, "sbp\n");
        return;
    }
    snprintf(msg, sizeof(msg), MSG_PLV, player->player_id, player->main_lvl);
    send_msg_client(client, msg);
    return;
}

void send_plv_all(individual_t *player, clients_management_t *client_manage)
{
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), MSG_PLV, player->player_id, player->main_lvl);
    send_msg_graphics(client_manage, msg);
    return;
}
