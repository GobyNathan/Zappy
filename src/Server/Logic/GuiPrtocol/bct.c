/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** bct
*/

#include "Server/gui_protocol.h"
#include "Server/utilities.h"
#include "Server/messages.h"
#include <string.h>
#include <stdlib.h>
#include <Server/server.h>
#include <stdio.h>

static int get_bct_y_coordinates(char *command)
{
    char *command_cpy = strdup(command);
    char *y_text = NULL;
    int y = 0;
    char *endptr = NULL;

    strtok(command_cpy, " \t");
    strtok(NULL, " \t");
    y_text = strtok(NULL, " \t");
    if (y_text == NULL){
        free(command_cpy);
        return -1;
    }
    y = (int) strtol(y_text, &endptr, 10);
    if (*endptr != '\0'){
        free(command_cpy);
        return -1;
    }
    free(command_cpy);
    return y;
}

static int get_bct_x_coordinates(char *command)
{
    char *command_cpy = strdup(command);
    char *x_text = NULL;
    int x = 0;
    char *endptr = NULL;

    strtok(command_cpy, " \t");
    x_text = strtok(NULL, " \t");
    if (x_text == NULL){
        free(command_cpy);
        return -1;
    }
    x = (int) strtol(x_text, &endptr, 10);
    if (*endptr != '\0'){
        free(command_cpy);
        return -1;
    }
    free(command_cpy);
    return x;
}

static int *count_elements(element_t *tile)
{
    element_t *tmp = tile;
    int *elems_nb = malloc(sizeof(int) * 7);

    for (int i = 0; i < 7; ++i)
        elems_nb[i] = 0;
    while (tmp != NULL){
        elems_nb[0] += tmp->elem_type == FOOD ? 1 : 0;
        elems_nb[1] += tmp->mineral_type == LINEMATE ? 1 : 0;
        elems_nb[2] += tmp->mineral_type == DERAUMERE ? 1 : 0;
        elems_nb[3] += tmp->mineral_type == SIBUR ? 1 : 0;
        elems_nb[4] += tmp->mineral_type == MENDIANE ? 1 : 0;
        elems_nb[5] += tmp->mineral_type == PHIRAS ? 1 : 0;
        elems_nb[6] += tmp->mineral_type == THYSTAME ? 1 : 0;
        tmp = tmp->next;
    }
    return elems_nb;
}

void bct_command(server_info_t *info, client_t *client, char *command)
{
    int x = get_bct_x_coordinates(command);
    int y = get_bct_y_coordinates(command);
    int *elems_nb = NULL;
    char msg[1024] = {0};

    if (x < 0 || y < 0 || x >= info->map->size_x || y >= info->map->size_y){
        send_msg_client(client, "sbp\n");
        return;
    }
    elems_nb = count_elements(info->map->map_array[y][x]);
    snprintf(msg, sizeof(msg), MSG_BCT, x, y, elems_nb[0], elems_nb[1],
        elems_nb[2], elems_nb[3], elems_nb[4], elems_nb[5], elems_nb[6]);
    send_msg_client(client, msg);
    free(elems_nb);
    return;
}

void send_bct_all(int x, int y, map_info_t *map,
    clients_management_t *client_manage)
{
    char msg[1024] = {0};
    int *elems_nb = NULL;

    elems_nb = count_elements(map->map_array[y][x]);
    snprintf(msg, sizeof(msg), MSG_BCT, x, y, elems_nb[0], elems_nb[1],
        elems_nb[2], elems_nb[3], elems_nb[4], elems_nb[5], elems_nb[6]);
    send_msg_graphics(client_manage, msg);
    free(elems_nb);
    return;
}
