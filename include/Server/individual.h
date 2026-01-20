/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#ifndef INCLUDED_INDIVIDUAL_H
    #define INCLUDED_INDIVIDUAL_H

    #include "client_info.h"
    #include "individual_info.h"
    #include "map_info.h"

individual_t *create_individual(int id, char *team_name, int map_x, int map_y);
bool remove_item_from_inventory(client_t *client, const char *item);
void add_item_to_inventory(client_t *client, const char *item);
void send_inventory_array(client_t *client, void *params);
bool client_has_object(client_t *client, const char *object);
void take_object(client_t *client, void *params);
void drop_object(client_t *client, void *params);
individual_t *get_individual_from_elem(element_t *elem);
void move_forward(client_t *client, void *params);
void turn_right(client_t *client, void *params);
void turn_left(client_t *client, void *params);
void individual_look(client_t *client, void *params);
void eject_individuals(client_t *author, void *params);
void individual_broadcast(client_t *client, void *params);
void individual_fork(client_t *client, void *params);

#endif /* INCLUDED_INDIVIDUAL_H */
