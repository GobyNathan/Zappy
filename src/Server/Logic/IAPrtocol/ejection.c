/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/ai_protocol_info.h"
#include "Server/individual_info.h"
#include "Server/map_info.h"
#include "Server/individual.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/client_info.h"
#include "Server/server_info.h"
#include "Server/utilities.h"
#include "Server/ia_messages.h"
#include "Server/gui_protocol.h"

int count_individuals_on_tile(client_t *author,
    clients_management_t *client_manage)
{
    int count = 0;
    client_t tmp = {0};

    if (!author || !author->individual)
        return 0;
    for (int i = 0; i < client_manage->len_clients; i++) {
        tmp = client_manage->clients[i];
        if (!tmp.individual)
            continue;
        if (tmp.individual->pos_x == author->individual->pos_x &&
            tmp.individual->pos_y == author->individual->pos_y) {
            count++;
        }
    }
    return count;
}

client_t **get_clients_on_same_tile(client_t *author,
    clients_management_t *client_manage)
{
    int individual_count = count_individuals_on_tile(author, client_manage);
    int idx = 0;
    client_t **on_tile = malloc(sizeof(client_t *) * (individual_count + 1));
    client_t *current = NULL;

    if (!on_tile)
        return NULL;
    on_tile[individual_count] = NULL;
    for (int i = 0; i < client_manage->len_clients; i++) {
        current = &client_manage->clients[i];
        if (!current->individual)
            continue;
        if (current->individual->pos_x == author->individual->pos_x &&
            current->individual->pos_y == author->individual->pos_y) {
            on_tile[idx] = current;
            idx++;
        }
    }
    return on_tile;
}

void eject_individuals(client_t *author, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;
    individual_t *indiv = author->individual;
    client_t **on_tile = get_clients_on_same_tile(author, args->client_manage);
    lookat_t tmp;

    if (!on_tile) {
        send_msg_client(author, MSG_KO);
        return;
    }
    for (int i = 0; on_tile[i]; i++) {
        if (on_tile[i]->individual == indiv)
            continue;
        tmp = on_tile[i]->individual->look_at;
        on_tile[i]->individual->look_at = indiv->look_at;
        move_forward(on_tile[i], params);
        on_tile[i]->individual->look_at = tmp;
    }
    send_msg_client(author, MSG_OK);
    send_pex_all(indiv, args->client_manage);
    free(on_tile);
}

void ai_eject(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&eject_individuals;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}
