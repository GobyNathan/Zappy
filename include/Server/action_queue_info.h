/*
** EPITECH PROJECT, 2025
** ConnectionInfo.h
** File description:
** contains the connection info struct
*/

#ifndef ACTION_QUEUE_INFO_H
    #define ACTION_QUEUE_INFO_H
    #include "Server/client_info.h"
    #define MAX_ACTIONS_PER_CLIENT 10

typedef void (*action_callback_t)(client_t *, void *);

typedef struct callback_info_s {
    action_callback_t callback;
    void *params;
    int scheduled_tick;
} callback_info_t;

typedef struct scheduled_action_s {
    int client_id;
    individual_t *p;
    struct callback_info_s *info;
    struct scheduled_action_s *next;
} scheduled_action_t;

#endif /* ACTION_QUEUE_INFO_H */
