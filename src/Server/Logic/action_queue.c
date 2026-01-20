/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** run_server.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Server/action_queue_info.h"
#include "Server/client_info.h"
#include "string.h"
#include <sys/time.h>
#include "Server/action_queue.h"
#include "Server/ai_protocol_info.h"
#include "Server/utilities.h"
#include "Server/ai_protocol.h"

static int count_client_actions(scheduled_action_t *head, int client_id)
{
    int count = 0;

    while (head != NULL) {
        if (head->client_id == client_id)
            count++;
        head = head->next;
    }
    return count;
}

bool maxed_out_queue_check(scheduled_action_t **head,
    scheduled_action_t *new_action)
{
    if (count_client_actions(*head, new_action->client_id)
        >= MAX_ACTIONS_PER_CLIENT) {
        free(new_action);
        return false;
    }
    return true;
}

void insert_action_in_queue(scheduled_action_t **head,
    scheduled_action_t *new_action)
{
    scheduled_action_t *curr = NULL;
    scheduled_action_t *prev = NULL;

    if (!head || !new_action)
        return;
    if (!maxed_out_queue_check(head, new_action))
        return;
    curr = *head;
    while (curr && curr->info->scheduled_tick
        <= new_action->info->scheduled_tick) {
        prev = curr;
        curr = curr->next;
    }
    new_action->next = curr;
    if (prev)
        prev->next = new_action;
    else
        *head = new_action;
}

scheduled_action_t *create_scheduled_action(client_t *client,
    callback_info_t *info)
{
    scheduled_action_t *action = malloc(sizeof(*action));

    action->client_id = client->id;
    action->p = client->individual;
    action->info = info;
    return action;
}

int get_last_scheduled_tick(client_t *client, int *current_tick,
    scheduled_action_t **head)
{
    scheduled_action_t *tmp = *head;

    while (tmp != NULL) {
        if (client->id == tmp->client_id)
            return tmp->info->scheduled_tick;
        tmp = tmp->next;
    }
    return *current_tick;
}

void tick_action_queue(scheduled_action_t **head, int current_tick)
{
    scheduled_action_t *tmp = NULL;
    client_t *client = NULL;

    while (*head && (*head)->info->scheduled_tick <= current_tick) {
        tmp = *head;
        *head = tmp->next;
        client = get_client_by_id(tmp->client_id,
            ((ai_command_args_t *)tmp->info->params)->client_manage);
        if (tmp->p->is_alive && client != NULL) {
            tmp->info->callback(client, tmp->info->params);
        }
        free(tmp);
    }
}
