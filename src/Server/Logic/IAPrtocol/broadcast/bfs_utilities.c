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
#include "Server/individual_info.h"
#include "Server/broadcast_utilities.h"
#include "Server/server_info.h"
#include "string.h"

bool **initialize_visited(int size_x, int size_y)
{
    bool **visited = calloc(size_y, sizeof(bool *));

    for (int i = 0; i < size_y; i++) {
        visited[i] = calloc(size_x, sizeof(bool));
    }
    return visited;
}

void process_clients_in_range(client_t *sender, bfs_context_t *bfs_ctx,
    queue_item_t current)
{
    client_t *receiver = NULL;

    for (int i = 0; i < bfs_ctx->args->client_manage->len_clients; ++i) {
        receiver = &bfs_ctx->args->client_manage->clients[i];
        if (receiver == bfs_ctx->ctx->author || !receiver->individual)
            continue;
        if (receiver->individual->pos_x != current.x ||
            receiver->individual->pos_y != current.y)
            continue;
        if (is_already_hit(bfs_ctx->ctx->hits, receiver))
            continue;
        bfs_ctx->ctx->hits = realloc_hits(bfs_ctx->ctx, sender, receiver,
            (int[2]){bfs_ctx->ctx->author->individual->pos_x,
                    bfs_ctx->ctx->author->individual->pos_y});
    }
}

void enqueue_neighbors(bfs_context_t *bfs_ctx, queue_item_t current,
    int x, int y)
{
    int nx = 0;
    int ny = 0;

    for (int i = 0; i < BFS_DIR_SIZE; i++) {
        nx = (x + bfs_dir[i][0] + bfs_ctx->args->map->size_x) %
            bfs_ctx->args->map->size_x;
        ny = (y + bfs_dir[i][1] + bfs_ctx->args->map->size_y) %
            bfs_ctx->args->map->size_y;
        if (!bfs_ctx->visited[ny][nx]) {
            bfs_ctx->visited[ny][nx] = true;
            bfs_ctx->queue[(*bfs_ctx->queue_end)] =
                (queue_item_t){nx, ny, current.distance + 1};
            (*bfs_ctx->queue_end)++;
        }
    }
}

void perform_bfs(client_t *sender, bfs_context_t *bfs_ctx)
{
    queue_item_t current = {0};

    while (*bfs_ctx->queue_start < *bfs_ctx->queue_end) {
        current = bfs_ctx->queue[(*bfs_ctx->queue_start)];
        (*bfs_ctx->queue_start)++;
        process_clients_in_range(sender, bfs_ctx, current);
        enqueue_neighbors(bfs_ctx, current, current.x, current.y);
    }
}

bfs_context_t *create_bfs_context(ai_command_args_t *args,
    broadcast_ctx_t *ctx)
{
    bfs_context_t *bfs = malloc(sizeof(bfs_context_t));

    bfs->args = args;
    bfs->ctx = ctx;
    bfs->queue = malloc(args->map->size_x * args->map->size_y
        * sizeof(queue_item_t));
    bfs->visited = initialize_visited(args->map->size_x,
        args->map->size_y);
    bfs->queue_start = malloc(sizeof(int));
    bfs->queue_end = malloc(sizeof(int));
    *bfs->queue_start = 0;
    *bfs->queue_end = 0;
    return bfs;
}
