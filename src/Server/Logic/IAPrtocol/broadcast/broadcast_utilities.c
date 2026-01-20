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
#include "Server/client_info.h"
#include "Server/broadcast_utilities.h"
#include "string.h"
#include "Server/utilities.h"
#include "Server/ia_messages.h"

static int get_hits_size(broadcast_hits_t **hits)
{
    int count = 0;

    if (!hits)
        return 0;
    while (hits[count])
        count++;
    return count;
}

bool is_already_hit(broadcast_hits_t **hits, client_t *client)
{
    if (!hits)
        return false;
    for (int i = 0; hits[i] != NULL; i++) {
        if (hits[i]->client == client)
            return true;
    }
    return false;
}

broadcast_hits_t **realloc_hits(broadcast_ctx_t *ctx, client_t *sender,
    client_t *hit, int cursor[2])
{
    int size = get_hits_size(ctx->hits);
    broadcast_hits_t **new_hits = realloc(ctx->hits,
        sizeof(broadcast_hits_t *) * (size + 2));

    if (!new_hits)
        return ctx->hits;
    new_hits[size] = malloc(sizeof(broadcast_hits_t));
    if (!new_hits[size])
        return new_hits;
    new_hits[size]->client = hit;
    if (sender->individual->pos_x == hit->individual->pos_x &&
        sender->individual->pos_y == hit->individual->pos_y)
        new_hits[size]->k = 0;
    else
        new_hits[size]->k = get_k_from_cursor(hit, ctx->map, cursor);
    new_hits[size + 1] = NULL;
    ctx->hits = new_hits;
    return new_hits;
}

broadcast_ctx_t create_broadcast_context(map_info_t *map,
    char *to_broadcast, client_t *author)
{
    return (broadcast_ctx_t){
        .map = map,
        .message = to_broadcast,
        .hits = NULL,
        .author = author
    };
}

void transmit_broadcast(client_t *client, broadcast_ctx_t *ctx)
{
    size_t len_broadcast = strlen(ctx->message) + 14;
    char *to_broadcast = malloc(len_broadcast);
    broadcast_hits_t *hit = NULL;

    if (!ctx->message) {
        free(to_broadcast);
        send_msg_client(client, MSG_OK);
        return;
    }
    if (ctx->hits) {
        for (int i = 0; ctx->hits[i] != NULL; i++) {
            hit = ctx->hits[i];
            snprintf(to_broadcast, len_broadcast,
                MSG_BROADCAST, hit->k, ctx->message);
            send_msg_client(ctx->hits[i]->client, to_broadcast);
        }
    }
    send_msg_client(client, MSG_OK);
    free(to_broadcast);
}

void normalize_direction(int *dx, int *dy)
{
    if (*dx != 0)
        *dx = *dx > 0 ? 1 : -1;
    if (*dy != 0)
        *dy = *dy > 0 ? 1 : -1;
}
