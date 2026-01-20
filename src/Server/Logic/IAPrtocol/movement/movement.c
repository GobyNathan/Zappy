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
#include "Server/client_info.h"
#include "Server/utilities.h"
#include "Server/gui_protocol.h"
#include <unistd.h>
#include <stdbool.h>

void move_forward(client_t *client, void *params)
{
    individual_t *p = client->individual;
    ai_command_args_t *args = (ai_command_args_t *)params;

    switch (p->look_at) {
        case NORTH:
            p->pos_y = (p->pos_y - 1 + args->map->size_y) % args->map->size_y;
            break;
        case EAST:
            p->pos_x = (p->pos_x + 1) % args->map->size_x;
            break;
        case SOUTH:
            p->pos_y = (p->pos_y + 1) % args->map->size_y;
            break;
        case WEST:
            p->pos_x = (p->pos_x - 1 + args->map->size_x) % args->map->size_x;
            break;
    }
    send_ppo_all(p, args->client_manage);
    send_msg_client(client, MSG_OK);
}

void turn_right(client_t *client, void *params)
{
    individual_t *individual = client->individual;
    ai_command_args_t *args = (ai_command_args_t *)params;

    switch (individual->look_at) {
        case NORTH:
            individual->look_at = EAST;
            break;
        case EAST:
            individual->look_at = SOUTH;
            break;
        case SOUTH:
            individual->look_at = WEST;
            break;
        case WEST:
            individual->look_at = NORTH;
            break;
    }
    send_msg_client(client, MSG_OK);
    send_ppo_all(individual, args->client_manage);
}

void turn_left(client_t *client, void *params)
{
    individual_t *individual = client->individual;
    ai_command_args_t *args = (ai_command_args_t *)params;

    switch (individual->look_at) {
        case NORTH:
            individual->look_at = WEST;
            break;
        case EAST:
            individual->look_at = NORTH;
            break;
        case SOUTH:
            individual->look_at = EAST;
            break;
        case WEST:
            individual->look_at = SOUTH;
            break;
    }
    send_msg_client(client, MSG_OK);
    send_ppo_all(individual, args->client_manage);
}
