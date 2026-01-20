/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#ifndef INCLUDED_MAP_H
    #define INCLUDED_MAP_H
    #include "map_info.h"
    #include "server_info.h"
    #include <sys/time.h>

map_info_t *init_map(int x, int y,
    int *current_tick, scheduled_action_t **action_queue);
void process_map_tile(map_info_t *map, float counter, int cursor[2],
    char *resource_name);
void append_to_element(element_t **tile, element_t *new_element);
void erase_from_element(element_t **tile, element_t *element_to_remove);
element_t *get_tile(map_info_t *map, int x, int y);
void refresh_map(map_info_t *map);
mineral_t get_mineral_type_from_name(const char *mineral_name);
element_t *get_element_from_tile(element_t *tile, const char *element);
element_t *get_resource_from_name(const char *resource_name);
void use_element_queue(map_info_t *map, element_t **queue_head,
    clients_management_t *client_manage);

#endif /* INCLUDED_MAP_H*/
