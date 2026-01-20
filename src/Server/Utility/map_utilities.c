/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#include "Server/map.h"
#include "Server/map_info.h"
#include <stdlib.h>
#include "Server/mineral_info.h"
#include <stdio.h>
#include "string.h"

void append_to_element(element_t **tile, element_t *new_element)
{
    element_t *current = NULL;

    if (*tile == NULL) {
        *tile = new_element;
        return;
    }
    current = *tile;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_element;
}

void erase_from_element(element_t **tile, element_t *element_to_remove)
{
    element_t *current = *tile;

    if (*tile == NULL || element_to_remove == NULL)
        return;
    if (*tile == element_to_remove) {
        *tile = (*tile)->next;
        free(element_to_remove);
        return;
    }
    while (current->next != NULL) {
        if (current->next == element_to_remove) {
            current->next = current->next->next;
            free(element_to_remove);
            return;
        }
        current = current->next;
    }
}

static element_t *get_food_from_tile(element_t *tile)
{
    element_t *current = tile;

    while (current != NULL) {
        if (current->elem_type == FOOD)
            return current;
        current = current->next;
    }
    return NULL;
}

static element_t *get_mineral_from_tile(element_t *tile,
    mineral_t mineral_type)
{
    element_t *current = tile;

    if (mineral_type == NONE)
        return NULL;
    while (current != NULL) {
        if (current->elem_type == MINERAL &&
            current->mineral_type == mineral_type) {
                return current;
        }
        current = current->next;
    }
    return NULL;
}

element_t *get_element_from_tile(element_t *tile, const char *element)
{
    if (strcmp(element, "food") == 0) {
        return get_food_from_tile(tile);
    } else {
        return get_mineral_from_tile(tile,
            get_mineral_type_from_name(element));
    }
    return NULL;
}

element_t *get_tile(map_info_t *map, int x, int y)
{
    x = (x + map->size_x) % map->size_x;
    y = (y + map->size_y) % map->size_y;
    return map->map_array[y][x];
}

static void add_ressource(map_info_t *map, int x, int y, char *resource_name)
{
    element_t *to_place = NULL;

    to_place = get_resource_from_name(resource_name);
    append_to_element(&(map->map_array[y][x]), to_place);
}

void refresh_map(map_info_t *map)
{
    int map_size = map->size_x * map->size_y;
    int nb_ressources = 0;

    for (int i = 0; i <= RESOURCE_NUM; i++){
        nb_ressources = map_size * resource_templates[i].density;
        for (int j = 0; j < nb_ressources; ++j){
            add_ressource(map, rand() % map->size_x, rand() % map->size_y,
            resource_templates[i].name);
        }
    }
}
