/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ai_protocol.h
*/

#ifndef AI_PROTOCOL_INFO_H
    #define AI_PROTOCOL_INFO_H
    #include "individual_info.h"
    #include "map_info.h"

typedef struct clients_management_a clients_management_t;

typedef struct ai_command_args_s {
    map_info_t *map;
    individual_t ***ptr_individuals;
    char **command_args;
    clients_management_t *client_manage;
} ai_command_args_t;

#endif /* AI_PROTOCOL_INFO_H */
