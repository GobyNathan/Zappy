/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Server/ai_protocol_info.h"
#include "Server/client_info.h"
#include "Server/ia_messages.h"
#include "Server/map_info.h"
#include "Server/individual_info.h"
#include "Server/individual.h"
#include "Server/map_info.h"
#include "Server/utilities.h"
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/broadcast_utilities.h"
#include "Server/gui_protocol.h"
#include "string.h"

static void free_hits(broadcast_hits_t **hits)
{
    for (int i = 0; hits && hits[i]; i++)
        free(hits[i]);
    free(hits);
}

static void free_broadcast_resource(bfs_context_t *bfs_ctx)
{
    for (int i = 0; i < bfs_ctx->args->map->size_y; i++) {
        free(bfs_ctx->visited[i]);
    }
    free(bfs_ctx->visited);
    free(bfs_ctx->queue);
    free_hits(bfs_ctx->ctx->hits);
}

static int calculate_shortest_distance(int coord1, int coord2, int map_size)
{
    int direct = coord2 - coord1;
    int wrap_positive = (coord2 + map_size) - coord1;
    int wrap_negative = coord2 - (coord1 + map_size);

    if (abs(direct) <= abs(wrap_positive) && abs(direct)
        <= abs(wrap_negative))
        return direct;
    if (abs(wrap_positive) <= abs(wrap_negative))
        return wrap_positive;
    return wrap_negative;
}

static int get_base_direction_from_normalized(int dx, int dy)
{
    if (dx == 0 && dy == -1)
        return 1;
    if (dx == -1 && dy == -1)
        return 2;
    if (dx == -1 && dy == 0)
        return 3;
    if (dx == -1 && dy == 1)
        return 4;
    if (dx == -1 && dy == 1)
        return 4;
    if (dx == 0 && dy == 1)
        return 5;
    if (dx == 1 && dy == 1)
        return 6;
    if (dx == 1 && dy == 0)
        return 7;
    if (dx == 1 && dy == -1)
        return 7;
    return 0;
}

static int get_base_direction(int dx, int dy)
{
    normalize_direction(&dx, &dy);
    return get_base_direction_from_normalized(dx, dy);
}

static int rotate_direction(int base_direction, lookat_t orientation_t)
{
    int rv = base_direction + (orientation_t *2);

    if (rv > 8)
        rv -= 8;
    return rv;
}

static int get_direction_from_offset(int dx, int dy, lookat_t orientation_t)
{
    int bd = get_base_direction(dx, dy);

    return rotate_direction(bd, orientation_t);
}

int get_k_from_cursor(client_t *receiver, map_info_t *map, int cursor[2])
{
    int dx = calculate_shortest_distance(receiver->individual->pos_x,
        cursor[0], map->size_x);
    int dy = calculate_shortest_distance(receiver->individual->pos_y,
        cursor[1], map->size_y);

    return get_direction_from_offset(dx, dy,
        receiver->individual->look_at);
}

void individual_broadcast(client_t *client, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;
    broadcast_ctx_t ctx = create_broadcast_context(args->map,
        args->command_args[1], client);
    bfs_context_t *bfs_ctx = create_bfs_context(args, &ctx);

    bfs_ctx->queue[(*bfs_ctx->queue_end)] =
        (queue_item_t){client->individual->pos_x,
        client->individual->pos_y, 0};
    (*bfs_ctx->queue_end)++;
    bfs_ctx->visited[client->individual->pos_y][client->individual->pos_x]
        = true;
    perform_bfs(client, bfs_ctx);
    transmit_broadcast(client, &ctx);
    send_pbc_all(client->individual, ctx.message, args->client_manage);
    free_broadcast_resource(bfs_ctx);
}

void ai_broadcast(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    if (!client->individual || !args->command_args[1]) {
        send_msg_client(client, MSG_KO);
        return;
    }
    cb_info->callback = (action_callback_t)&individual_broadcast;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}
