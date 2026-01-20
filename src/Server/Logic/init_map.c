/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#include "Server/mineral_info.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Server/map.h"
#include "Server/action_queue_info.h"
#include <sys/time.h>

mineral_t get_mineral_type_from_name(const char *mineral_name)
{
    if (strcmp(mineral_name, "linemate") == 0)
        return LINEMATE;
    if (strcmp(mineral_name, "deraumere") == 0)
        return DERAUMERE;
    if (strcmp(mineral_name, "sibur") == 0)
        return SIBUR;
    if (strcmp(mineral_name, "mendiane") == 0)
        return MENDIANE;
    if (strcmp(mineral_name, "phiras") == 0)
        return PHIRAS;
    if (strcmp(mineral_name, "thystame") == 0)
        return THYSTAME;
    return NONE;
}

static element_t *create_food_element(void)
{
    element_t *el = malloc(sizeof(element_t));

    if (!el)
        return NULL;
    el->elem_type = FOOD;
    el->mineral_type = NONE;
    el->elem = NULL;
    el->next = NULL;
    return el;
}

static element_t *create_mineral_element(mineral_t type)
{
    element_t *el = malloc(sizeof(element_t));

    if (!el)
        return NULL;
    el->elem_type = MINERAL;
    el->mineral_type = type;
    el->elem = NULL;
    el->next = NULL;
    return el;
}

element_t *get_resource_from_name(const char *resource_name)
{
    if (strcmp(resource_name, "food") == 0) {
        return create_food_element();
    } else {
        return create_mineral_element(
            get_mineral_type_from_name(resource_name));
    }
    return NULL;
}

element_t ***init_map_array(int x, int y)
{
    element_t ***map_array = malloc(sizeof(element_t **) * y);

    if (!map_array)
        return NULL;
    for (int i = 0; i < y; i++) {
        map_array[i] = malloc(sizeof(element_t *) * x);
        if (!map_array[i])
            return NULL;
        for (int j = 0; j < x; j++) {
            map_array[i][j] = NULL;
        }
    }
    return map_array;
}

map_info_t *init_map(int x, int y, int *current_tick,
    scheduled_action_t **action_queue)
{
    map_info_t *map = malloc(sizeof(map_info_t));

    map->size_x = x;
    map->size_y = y;
    map->map_array = init_map_array(x, y);
    map->current_tick = current_tick;
    map->action_queue = action_queue;
    map->element_queue = NULL;
    refresh_map(map);
    return map;
}
