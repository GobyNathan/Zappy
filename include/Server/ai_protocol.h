/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ai_protocol.h
*/

#ifndef ACTION_WRAPPER_H
    #define ACTION_WRAPPER_H
    #include "ai_protocol_info.h"
    #include "client_info.h"

void ai_move_forward(client_t *client, ai_command_args_t *args, int delay);
void ai_turn_right(client_t *client, ai_command_args_t *args, int delay);
void ai_turn_left(client_t *client, ai_command_args_t *args, int delay);
void ai_look(client_t *client, ai_command_args_t *args, int delay);
void ai_inventory(client_t *client, ai_command_args_t *args, int delay);
void ai_broadcast(client_t *client, ai_command_args_t *args, int delay);
void ai_fork(client_t *client, ai_command_args_t *args, int delay);
void ai_eject(client_t *client, ai_command_args_t *args, int delay);
void ai_take_object(client_t *client, ai_command_args_t *args, int delay);
void ai_set_object(client_t *client, ai_command_args_t *args, int delay);
void ai_incantation(client_t *client, ai_command_args_t *args, int delay);

#endif /* ACTION_WRAPPER_H */
