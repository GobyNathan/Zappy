/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** broadcast_utilities.h
*/

#ifndef BROADCAST_UTILS_H
    #define BROADCAST_UTILS_H
    #include "ai_protocol_info.h"
    #include "client_info.h"
    #define BFS_DIR_SIZE 8
    #define MAX_BROADCAST_DISTANCE 1000

typedef struct {
    int x, y, distance;
} queue_item_t;

static const int bfs_dir[8][2] = {
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0},
    {1, 1},
    {1, -1},
    {-1, 1},
    {-1, -1}
};

typedef struct broadcast_hits_s {
    client_t *client;
    int k;
} broadcast_hits_t;

typedef struct broadcast_ctx_s {
    map_info_t *map;
    char *message;
    broadcast_hits_t **hits;
    client_t *author;
} broadcast_ctx_t;

typedef struct bfs_context_s {
    ai_command_args_t *args;
    queue_item_t *queue;
    int *queue_start;
    int *queue_end;
    bool **visited;
    broadcast_ctx_t *ctx;
} bfs_context_t;

bool is_already_hit(broadcast_hits_t **hits, client_t *client);
broadcast_hits_t **realloc_hits(broadcast_ctx_t *ctx,
    client_t *sender, client_t *hit, int cursor[2]);
bool **initialize_visited(int size_x, int size_y);
void process_clients_in_range(client_t *client, bfs_context_t *bfs_ctx,
    queue_item_t current);
void enqueue_neighbors(bfs_context_t *bfs_ctx,
    queue_item_t current, int x, int y);
void perform_bfs(client_t *client, bfs_context_t *bfs_ctx);
bfs_context_t *create_bfs_context(ai_command_args_t *args,
    broadcast_ctx_t *ctx);
broadcast_ctx_t create_broadcast_context(map_info_t *map,
    char *to_broadcast, client_t *author);
int get_k_from_cursor(client_t *receiver, map_info_t *map, int cursor[2]);
void transmit_broadcast(client_t *client, broadcast_ctx_t *ctx);
void normalize_direction(int *dx, int *dy);

#endif /* BROADCAST_UTILS_H */
