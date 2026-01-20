/*
** EPITECH PROJECT, 2025
** ConnectionInfo.h
** File description:
** contains the connection info struct
*/

#ifndef SERVER_INFO_H
    #define SERVER_INFO_H
    #include "client_info.h"
    #include "action_queue_info.h"
    #include "individual_info.h"
    #include "ai_protocol.h"
    #include <sys/time.h>
    #define AI_COMMAND_NB 11

typedef struct clients_management_a {
    struct pollfd *fds;
    client_t *clients;
    int len_fds;
    int len_clients;
    int all_time_clients;
} clients_management_t;

typedef struct server_info_a {
    int port;
    char **names;
    int original_slots_nb;
    int freq;
    int current_tick;
    struct scheduled_action_s *action_queue;
    individual_t **individuals;
    map_info_t *map;
} server_info_t;

typedef struct ai_command_info_s {
    const char *name;
    double delay_factor;
    void (*callback)(client_t *, ai_command_args_t *, int delay);
} ai_command_info_t;

static const ai_command_info_t AI_COMMANDS[] = {
    {"Forward", 7.0, &ai_move_forward},
    {"Right", 7.0, &ai_turn_right},
    {"Left", 7.0, &ai_turn_left},
    {"Look", 7.0, &ai_look},
    {"Inventory", 1.0, &ai_inventory},
    {"Broadcast", 7.0, &ai_broadcast},
    {"Fork", 42.0, &ai_fork},
    {"Eject", 7.0, &ai_eject},
    {"Take", 7.0, &ai_take_object},
    {"Set", 7.0, &ai_set_object},
    {"Incantation", 300.0, &ai_incantation}
};

#endif /* SERVER_INFO_H */
