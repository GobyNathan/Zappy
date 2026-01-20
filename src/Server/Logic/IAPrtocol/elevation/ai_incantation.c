/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** elevation_group.c
*/

#include "Server/ai_protocol_info.h"
#include "Server/ai_protocol.h"
#include "Server/elevation_utilities.h"
#include "Server/action_queue.h"
#include <stddef.h>
#include <stdlib.h>

void ai_incantation(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&process_elevation;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
    args->map->current_tick, args->map->action_queue) + ELEVATION_OFFSET;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
    (void)delay;
}
