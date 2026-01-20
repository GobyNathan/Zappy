/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** elevation_utilities
*/

#ifndef INCLUDED_ELEVATION_UTILITIES_H
    #define INCLUDED_ELEVATION_UTILITIES_H
    #include "ai_protocol_info.h"
    #define ELEVATION_OFFSET 10

bool elevation_group_stayed(int *elevation_group, int group_size,
    individual_t **all, individual_t *individual);
bool last_to_elevate(int *elevation_group, int group_size,
    individual_t **all, individual_t *individual);
bool in_group(int *group, int group_size, int player_id);
int *get_elevation_group(individual_t *indiv, individual_t **all);
void msg_elevation_begin(int *group, int group_size,
    clients_management_t *manage);
void msg_elevation_end(client_t *client);
bool check_group(client_t *client, int *group, ai_command_args_t *args,
    callback_info_t *cb_info);
void process_elevation(client_t *client, void *params);

#endif
