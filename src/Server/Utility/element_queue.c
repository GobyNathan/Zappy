/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#include "Server/map.h"
#include "Server/map_info.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "Server/gui_protocol.h"
#include "Server/server_info.h"

void use_element_queue(map_info_t *map, element_t **queue_head,
    clients_management_t *client_manage)
{
    element_t *tmp = *queue_head;
    element_t *next = NULL;
    int x = 0;
    int y = 0;

    while (tmp) {
        next = tmp->next;
        x = rand() % map->size_x;
        y = rand() % map->size_y;
        tmp->next = NULL;
        append_to_element(&(map->map_array[y][x]), tmp);
        send_bct_all(x, y, map, client_manage);
        tmp = next;
    }
    *queue_head = NULL;
}
