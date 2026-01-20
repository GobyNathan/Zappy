/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/individual_info.h"
#include "Server/map_info.h"
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

individual_t *get_individual_from_elem(element_t *elem)
{
    if (elem->elem_type == INDIVIDUAL)
        return (individual_t *)elem->elem;
    return NULL;
}

bool client_has_object(client_t *client, const char *object)
{
    if (strcmp(object, "linemate") == 0)
        return client->individual->inventory->linemate > 0;
    if (strcmp(object, "deraumere") == 0)
        return client->individual->inventory->deraumere > 0;
    if (strcmp(object, "sibur") == 0)
        return client->individual->inventory->sibur > 0;
    if (strcmp(object, "mendiane") == 0)
        return client->individual->inventory->mendiane > 0;
    if (strcmp(object, "phiras") == 0)
        return client->individual->inventory->phiras > 0;
    if (strcmp(object, "thystame") == 0)
        return client->individual->inventory->thystame > 0;
    if (strcmp(object, "food") == 0)
        return client->individual->main_lvl > 0;
    return false;
}

static inventory_t *create_inventory(void)
{
    inventory_t *inventory = malloc(sizeof(inventory_t));

    inventory->deraumere = 0;
    inventory->linemate = 0;
    inventory->mendiane = 0;
    inventory->phiras = 0;
    inventory->sibur = 0;
    inventory->thystame = 0;
    return inventory;
}

individual_t *create_individual(int id, char *team_name, int x, int y)
{
    individual_t *individual = malloc(sizeof(individual_t));

    individual->player_id = id;
    individual->food_lvl = 10;
    individual->main_lvl = 1;
    individual->vision_range = DEFAULT_VISION;
    individual->pos_x = x;
    individual->pos_y = y;
    individual->look_at = SOUTH;
    individual->elevation_group = NULL;
    individual->elevation_group_size = 0;
    individual->is_alive = false;
    individual->is_egg = true;
    individual->inventory = create_inventory();
    individual->team = strdup(team_name);
    return individual;
}
