/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/ai_protocol_info.h"
#include "Server/ia_messages.h"
#include "Server/individual_info.h"
#include "Server/map_info.h"
#include "Server/map.h"
#include "Server/client_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/utilities.h"

static int calculate_total_tiles(int vision_range)
{
    int total_tiles = 0;

    for (int dist = 0; dist <= vision_range; dist++) {
        total_tiles += (2 * dist + 1);
    }
    return total_tiles;
}

const char *get_mineral_name_from_type(mineral_t type)
{
    switch (type) {
        case LINEMATE:
            return "linemate";
        case DERAUMERE:
            return "deraumere";
        case SIBUR:
            return "sibur";
        case MENDIANE:
            return "mendiane";
        case PHIRAS:
            return "phiras";
        case THYSTAME:
            return "thystame";
        default:
            return "";
    }
}

static void build_tile_string(char *result, element_t *current, bool init)
{
    bool first = true;

    if (!init) {
        result[0] = '\0';
    }
    while (current) {
        if (!first)
            strcat(result, " ");
        if (current->elem_type == FOOD)
            strcat(result, "food");
        if (current->elem_type == MINERAL)
            strcat(result, get_mineral_name_from_type(current->mineral_type));
        first = false;
        current = current->next;
    }
}

static look_at_info_t get_direction(lookat_t look_at)
{
    switch (look_at) {
        case NORTH:
            return (look_at_info_t){-1, 0, 0, 1};
        case SOUTH:
            return (look_at_info_t){1, 0, 0, -1};
        case EAST:
            return (look_at_info_t){0, 1, 1, 0};
        case WEST:
            return (look_at_info_t){0, -1, -1, 0};
        default:
            return (look_at_info_t){0, 0, 0, 0};
    }
}

static void build_tile_for_location(char **vision, int *tile_index,
    map_info_t *map, int idx[2])
{
    element_t *tile_elements = get_tile(map, idx[0], idx[1]);
    char *tile_str = malloc(256);

    if ((*tile_index) == 0) {
        tile_str[0] = '\0';
        strcat(tile_str, "player ");
        build_tile_string(tile_str, tile_elements, true);
    } else {
        build_tile_string(tile_str, tile_elements, false);
    }
    vision[(*tile_index)] = tile_str;
    (*tile_index)++;
}

static void send_look_result(client_t *client, char **vision)
{
    char *look_result = join_strings(vision, ", ");
    size_t len = strlen(look_result) + 6;
    char *msg = malloc(len);

    snprintf(msg, len, MSG_LOOK, look_result);
    send_msg_client(client, msg);
    free(msg);
    free(look_result);
}

char *get_look_at(client_t *client)
{
    if (client->individual->look_at == NORTH)
        return "north";
    if (client->individual->look_at == EAST)
        return "east";
    if (client->individual->look_at == WEST)
        return "west";
    if (client->individual->look_at == SOUTH)
        return "south";
    return "";
}

void tile_depth_scan(client_t *client, ai_command_args_t *args,
    int *tile_index)
{
    individual_t *individual = client->individual;
    int total_tiles = calculate_total_tiles(individual->vision_range);
    char **vision = malloc(sizeof(char *) * (total_tiles + 1));
    look_at_info_t dir = get_direction(individual->look_at);
    int cx = 0;
    int cy = 0;
    int cur[2] = {0};

    for (int depth = 0; depth <= individual->vision_range; ++depth) {
        cx = individual->pos_x + (dir.fx * depth);
        cy = individual->pos_y + (dir.fy * depth);
        for (int i = -depth; i <= depth; ++i) {
            cur[0] = (cx + i * dir.lx + args->map->size_x) % args->map->size_x;
            cur[1] = (cy + i * dir.ly + args->map->size_y) % args->map->size_y;
            build_tile_for_location(vision, tile_index, args->map, cur);
        }
    }
    vision[(*tile_index)] = NULL;
    send_look_result(client, vision);
}

void individual_look(client_t *client, void *params)
{
    ai_command_args_t *args = (ai_command_args_t *)params;
    int tile_index = 0;

    tile_depth_scan(client, args, &tile_index);
}

void ai_look(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&individual_look;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}
