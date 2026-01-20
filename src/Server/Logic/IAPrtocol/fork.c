/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/ai_protocol_info.h"
#include "Server/gui_protocol.h"
#include "Server/ia_messages.h"
#include "Server/individual.h"
#include "Server/map_info.h"
#include "Server/map_info.h"
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/utilities.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int count_individuals(individual_t **all_individuals)
{
    int count = 0;

    while (all_individuals[count] != NULL) {
        count++;
    }
    return count;
}

void individual_fork(client_t *client, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;
    int new_size = count_individuals(*args->ptr_individuals) + 1;
    individual_t **temp = realloc(*args->ptr_individuals,
        (new_size + 1) * sizeof(individual_t *));

    if (temp == NULL)
        return;
    *args->ptr_individuals = temp;
    (*args->ptr_individuals)[new_size - 1] = create_individual(new_size - 1,
        client->team, client->individual->pos_x, client->individual->pos_y);
    (*args->ptr_individuals)[new_size] = NULL;
    send_enw_all((*args->ptr_individuals)[new_size - 1], args->client_manage,
        client->individual->player_id);
    send_msg_client(client, MSG_OK);
}

void ai_fork(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&individual_fork;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
    send_pfk_all(client->individual, args->client_manage);
}
