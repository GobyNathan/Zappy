/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/ai_protocol_info.h"
#include "Server/map_info.h"
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/client_info.h"
#include "Server/individual.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

void ai_move_forward(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&move_forward;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}

void ai_turn_right(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&turn_right;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}

void ai_turn_left(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&turn_left;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}
