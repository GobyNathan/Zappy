/*
** EPITECH PROJECT, 2025
** ConnectionInfo.h
** File description:
** contains the connection info struct
*/

#ifndef ACTION_QUEUE_H
    #define ACTION_QUEUE_H
    #include "Server/client_info.h"
    #include "action_queue_info.h"

scheduled_action_t *create_scheduled_action(client_t *p,
    callback_info_t *info);
void tick_action_queue(scheduled_action_t **head, int current_tick);
void insert_action_in_queue(scheduled_action_t **head,
    scheduled_action_t *new_action);
int get_last_scheduled_tick(client_t *client, int *current_tick,
    scheduled_action_t **head);

#endif /* ACTION_QUEUE_H */
