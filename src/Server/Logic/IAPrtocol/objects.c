/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/ai_protocol_info.h"
#include "Server/ia_messages.h"
#include "Server/individual_info.h"
#include "Server/individual.h"
#include "Server/map_info.h"
#include "Server/map.h"
#include "Server/map_info.h"
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/utilities.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "Server/gui_protocol.h"
#include <stdio.h>

void take_object(client_t *client, void *params)
{
    individual_t *p = client->individual;
    ai_command_args_t *args = (ai_command_args_t *)params;
    element_t **tile_ptr = &args->map->map_array[p->pos_y][p->pos_x];
    element_t *element = get_element_from_tile(*tile_ptr,
        args->command_args[1]);

    if (!element)
        return send_msg_client(client, MSG_KO);
    if (strcmp(args->command_args[1], "food") == 0)
        client->individual->food_lvl += 1;
    else
        add_item_to_inventory(client, args->command_args[1]);
    erase_from_element(tile_ptr, element);
    element = get_resource_from_name(args->command_args[1]);
    append_to_element(&args->map->element_queue, element);
    send_pgt_all(p, args->command_args[1], args->client_manage);
    send_pin_all(p, args->client_manage);
    send_bct_all(p->pos_x, p->pos_y, args->map, args->client_manage);
    send_msg_client(client, MSG_OK);
}

void drop_object(client_t *client, void *params)
{
    individual_t *p = client->individual;
    ai_command_args_t *args = (ai_command_args_t *)params;
    element_t **tile = &(args->map->map_array[p->pos_y][p->pos_x]);
    element_t *to_drop = NULL;

    if (!client_has_object(client, args->command_args[1])) {
        send_msg_client(client, MSG_KO);
        return;
    }
    remove_item_from_inventory(client, args->command_args[1]);
    to_drop = get_element_from_tile(args->map->element_queue,
        args->command_args[1]);
    if (to_drop)
        erase_from_element(&args->map->element_queue, to_drop);
    to_drop = get_resource_from_name(args->command_args[1]);
    append_to_element(tile, to_drop);
    send_pdr_all(p, args->command_args[1], args->client_manage);
    send_pin_all(p, args->client_manage);
    send_bct_all(p->pos_x, p->pos_y, args->map, args->client_manage);
    send_msg_client(client, MSG_OK);
}

void ai_set_object(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&drop_object;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}

void ai_take_object(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&take_object;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}
