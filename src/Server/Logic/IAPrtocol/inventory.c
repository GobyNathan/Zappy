/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include "Server/ia_messages.h"
#include "Server/individual.h"
#include "Server/individual_info.h"
#include "Server/client_info.h"
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Server/ai_protocol.h"
#include "Server/action_queue_info.h"
#include "Server/action_queue.h"
#include "Server/utilities.h"

static char **fetch_inventory_array(client_t *client)
{
    char **inventory_array = malloc(8 * sizeof(char *));

    asprintf(&inventory_array[0],
        "food %d", client->individual->food_lvl);
    asprintf(&inventory_array[1],
        " linemate %d", client->individual->inventory->linemate);
    asprintf(&inventory_array[2],
        " deraumere %d", client->individual->inventory->deraumere);
    asprintf(&inventory_array[3],
        " sibur %d", client->individual->inventory->sibur);
    asprintf(&inventory_array[4],
        " mendiane %d", client->individual->inventory->mendiane);
    asprintf(&inventory_array[5],
        " phiras %d", client->individual->inventory->phiras);
    asprintf(&inventory_array[6],
        " thystame %d", client->individual->inventory->thystame);
    inventory_array[7] = NULL;
    return inventory_array;
}

void send_inventory_array(client_t *client, void *params)
{
    char **inventory_array = fetch_inventory_array(client);
    char *inventory_list = join_strings(inventory_array, ",");
    size_t len = strlen(inventory_list) + 6;
    char *msg = malloc(len);

    (void)params;
    snprintf(msg, len, MSG_INV, inventory_list);
    send_msg_client(client, msg);
    free(msg);
    free(inventory_list);
}

void add_item_to_inventory(client_t *client, const char *item)
{
    if (strcmp(item, "linemate") == 0)
        client->individual->inventory->linemate++;
    if (strcmp(item, "deraumere") == 0)
        client->individual->inventory->deraumere++;
    if (strcmp(item, "sibur") == 0)
        client->individual->inventory->sibur++;
    if (strcmp(item, "mendiane") == 0)
        client->individual->inventory->mendiane++;
    if (strcmp(item, "phiras") == 0)
        client->individual->inventory->phiras++;
    if (strcmp(item, "thystame") == 0)
        client->individual->inventory->thystame++;
}

static bool remove_item_from_inventory_bis(individual_t *individual,
    const char *item)
{
    if (strcmp(item, "phiras") == 0 &&
        individual->inventory->phiras > 0) {
        individual->inventory->phiras--;
        return true;
    }
    if (strcmp(item, "thystame") == 0 &&
        individual->inventory->thystame > 0) {
        individual->inventory->thystame--;
        return true;
    }
    if (strcmp(item, "mendiane") == 0 &&
        individual->inventory->mendiane > 0) {
        individual->inventory->mendiane--;
        return true;
    }
    if (strcmp(item, "food") == 0 && individual->food_lvl > 0) {
        individual->food_lvl--;
        return true;
    }
    return false;
}

bool remove_item_from_inventory(client_t *client, const char *item)
{
    if (strcmp(item, "linemate") == 0 &&
        client->individual->inventory->linemate > 0) {
        client->individual->inventory->linemate--;
        return true;
    }
    if (strcmp(item, "deraumere") == 0 &&
        client->individual->inventory->deraumere > 0) {
        client->individual->inventory->deraumere--;
        return true;
    }
    if (strcmp(item, "sibur") == 0 &&
        client->individual->inventory->sibur > 0) {
        client->individual->inventory->sibur--;
        return true;
    }
    return remove_item_from_inventory_bis(client->individual, item);
}

void print_inventory(const individual_t *individual)
{
    printf("Inventory:\n");
    printf("  Linemate: %d\n", individual->inventory->linemate);
    printf("  Deraumere: %d\n", individual->inventory->deraumere);
    printf("  Sibur: %d\n", individual->inventory->sibur);
    printf("  Mendiane: %d\n", individual->inventory->mendiane);
    printf("  Phiras: %d\n", individual->inventory->phiras);
    printf("  Thystame: %d\n", individual->inventory->thystame);
}

void ai_inventory(client_t *client, ai_command_args_t *args, int delay)
{
    callback_info_t *cb_info = malloc(sizeof(callback_info_t));
    scheduled_action_t *action = NULL;

    cb_info->callback = (action_callback_t)&send_inventory_array;
    cb_info->params = (void *)args;
    cb_info->scheduled_tick = get_last_scheduled_tick(client,
        args->map->current_tick, args->map->action_queue) + delay;
    action = create_scheduled_action(client, cb_info);
    insert_action_in_queue(args->map->action_queue, action);
}
