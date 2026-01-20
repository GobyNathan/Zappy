/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Server/action_queue_info.h"
#include "Server/ai_protocol_info.h"
#include "Server/ia_messages.h"
#include "Server/individual_info.h"
#include "Server/map.h"
#include "Server/map_info.h"
#include "Server/mineral_info.h"
#include "Server/utilities.h"
#include "Server/client_info.h"
#include "Server/action_queue.h"
#include "Server/elevation_utilities.h"
#include "Server/gui_protocol.h"

static void count_tile_stones(element_t *tile, int *found)
{
    while (tile) {
        if (tile->elem_type != MINERAL) {
            tile = tile->next;
            continue;
        }
        found[tile->mineral_type]++;
        tile = tile->next;
    }
}

static bool correct_stones_number(client_t *client, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;
    individual_t *indiv = client->individual;
    int found[6] = {0};
    elevation_req_t req;
    element_t *tile = NULL;

    req = ELEVATION_REQUIREMENTS[indiv->main_lvl - 1];
    tile = get_tile(args->map, indiv->pos_x, indiv->pos_y);
    count_tile_stones(tile, found);
    for (int i = 0; i < 6; i++) {
        if (found[i] < req.stones[i]) {
            return false;
        }
    }
    return true;
}

static void use_resources_on_tile(client_t *client, ai_command_args_t *args)
{
    individual_t *p = client->individual;
    element_t **ptr_tile = &(args->map->map_array[p->pos_y][p->pos_x]);
    elevation_req_t req = ELEVATION_REQUIREMENTS[p->main_lvl - 1];
    element_t *cur = NULL;
    mineral_t tmp = {0};
    element_t *replace = NULL;

    for (int i = 0; i < 6; i++) {
        tmp = (mineral_t)i;
        for (int j = 0; j < req.stones[i]; j++) {
            cur = get_element_from_tile(*ptr_tile,
            get_mineral_name_from_type(tmp));
            erase_from_element(ptr_tile, cur);
            replace = get_resource_from_name(get_mineral_name_from_type(tmp));
            append_to_element(&args->map->element_queue, replace);
            send_bct_all(p->pos_x, p->pos_y, args->map, args->client_manage);
        }
    }
}

static bool can_finally_elevate(client_t *client, ai_command_args_t *args)
{
    individual_t *p = client->individual;

    return elevation_group_stayed(p->elevation_group, p->elevation_group_size,
        *args->ptr_individuals, p) && correct_stones_number(client, args);
}

static void elevation_success(client_t *client, ai_command_args_t *args)
{
    individual_t *p = client->individual;

    if (last_to_elevate(p->elevation_group, p->elevation_group_size,
        *args->ptr_individuals, p)){
            use_resources_on_tile(client, args);
            free(client->individual->elevation_group);
            send_pie_all(client->individual->pos_x, client->individual->pos_y,
                true, args->client_manage);
    }
    client->individual->main_lvl++;
    client->individual->vision_range++;
    client->individual->elevation_group = NULL;
    client->individual->elevation_group_size = 0;
    msg_elevation_end(client);
    send_plv_all(p, args->client_manage);
}

static void elevation_fail(client_t *client, ai_command_args_t *args)
{
    individual_t *p = client->individual;

    if (last_to_elevate(p->elevation_group, p->elevation_group_size,
        *args->ptr_individuals, p)){
        free(client->individual->elevation_group);
        send_pie_all(client->individual->pos_x, client->individual->pos_y,
            false, args->client_manage);
    }
    client->individual->elevation_group = NULL;
    client->individual->elevation_group_size = 0;
    send_msg_client(client, MSG_KO);
}

static void elevate_final(client_t *client, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;

    if (!can_finally_elevate(client, args))
        return elevation_fail(client, args);
    return elevation_success(client, args);
}

static void lauch_elevation(client_t *client, ai_command_args_t *args,
    callback_info_t *cb_info, int *group)
{
    client_t *tmp = NULL;
    scheduled_action_t *action = NULL;

    for (int i = 0; i < args->client_manage->len_clients; ++i){
        tmp = &args->client_manage->clients[i];
        if (tmp->individual == NULL)
            continue;
        if (in_group(group, client->individual->elevation_group_size,
            tmp->individual->player_id)){
            tmp->individual->elevation_group = group;
            tmp->individual->elevation_group_size =
            client->individual->elevation_group_size;
            action = create_scheduled_action(tmp, cb_info);
            insert_action_in_queue(args->map->action_queue, action);
        }
    }
    msg_elevation_begin(group, client->individual->elevation_group_size,
        args->client_manage);
}

bool check_group(client_t *client, int *group, ai_command_args_t *args,
    callback_info_t *cb_info)
{
    if (group == NULL || !correct_stones_number(client, args)){
            free(cb_info);
            client->individual->elevation_group = NULL;
            client->individual->elevation_group_size = 0;
            send_msg_client(client, MSG_KO);
            return false;
    }
    return true;
}

void process_elevation(client_t *client, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    int *group = NULL;
    int curr_lv = client->individual->main_lvl;

    if (curr_lv == MAX_LEVEL){
        free(cb_info);
        return send_msg_client(client, MSG_KO);
    }
    group = get_elevation_group(client->individual, *args->ptr_individuals);
    if (!check_group(client, group, args, cb_info))
        return;
    cb_info->callback = (action_callback_t)&elevate_final;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + 300;
    lauch_elevation(client, args, cb_info, group);
}
