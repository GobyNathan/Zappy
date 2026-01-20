/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** manage_client_input.c
*/

#include <unistd.h>
#include "Server/ai_protocol_info.h"
#include "Server/server_info.h"
#include "Server/utilities.h"
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "Server/gui_protocol.h"
#include <stdio.h>
#include "Server/server.h"

static int available_slots(server_info_t *info, char *team_name)
{
    int slots_available = 0;

    for (int i = 0; info->individuals[i] != NULL; ++i){
        if (info->individuals[i]->is_egg &&
            strcmp(info->individuals[i]->team, team_name) == 0)
            slots_available++;
    }
    return slots_available;
}

static individual_t *get_available_egg(server_info_t *info, char *team_name)
{
    for (int i = 0; info->individuals[i] != NULL; ++i){
        if (info->individuals[i]->is_egg &&
            strcmp(info->individuals[i]->team, team_name) == 0)
            return info->individuals[i];
    }
    return NULL;
}

static void send_opening_msg(server_info_t *info, client_t *client,
    int team_index)
{
    int slots_available = available_slots(info, info->names[team_index]);
    char msg[1024] = {0};

    snprintf(msg, sizeof(msg), "%d\n%d %d\n", slots_available,
    info->map->size_x, info->map->size_y);
    send_msg_client(client, msg);
}

static void read_client_input(clients_management_t *client_manage,
    int client_index, int *removed)
{
    client_t *client = &client_manage->clients[client_index];
    int curr_buff_size = strlen(client->buffer);
    int read_size = 0;
    int bytes_read = 0;

    if (client->buffer[0] == '\0')
        memset(client->buffer, 0, CLIENT_BUFFER_SIZE);
    curr_buff_size = strlen(client->buffer);
    if (curr_buff_size >= CLIENT_BUFFER_SIZE - 1)
        return;
    read_size = CLIENT_BUFFER_SIZE - 1 - curr_buff_size;
    bytes_read = read(client->sockfd, client->buffer +
        curr_buff_size, read_size);
    if (bytes_read > 0) {
        client->buffer[curr_buff_size + bytes_read] = '\0';
    } else {
        remove_client(client_manage, client_index);
        *removed = 1;
    }
}

static void assign_new_player(server_info_t *info,
    clients_management_t *client_manage, client_t *client, int team_index)
{
    individual_t *egg = get_available_egg(info, info->names[team_index]);

    if (egg == NULL)
        return;
    client->individual = egg;
    client->individual->is_alive = true;
    client->individual->is_egg = false;
    client->team = strdup(info->names[team_index]);
    send_opening_msg(info, client, team_index);
    send_pnw_all(client->individual, client_manage);
    send_ebo_all(client->individual, client_manage);
    send_pin_all(client->individual, client_manage);
}

static void handle_unassigned_client(server_info_t *info,
    clients_management_t *client_manage, client_t *client)
{
    char *team_name = substring_until(client->buffer, '\n');

    if (strlen(team_name) == strlen(client->buffer)){
        free(team_name);
        return;
    }
    for (int i = 0; info->names[i] != NULL; ++i){
        if (strcmp(info->names[i], team_name) == 0)
            assign_new_player(info, client_manage, client, i);
    }
    if (strcmp(team_name, "GRAPHIC") == 0)
        new_gui_client(info, client);
    if (strcmp(client->team, "NOTEAM") == 0)
        send_msg_client(client, "ko\n");
    pop_command(client);
    free(team_name);
}

void empty_gui_cmd_queue(client_t *client, server_info_t *info)
{
    int i = 0;

    while (strlen(client->buffer) > 0 && i < 5) {
        handle_gui_client(info, client);
        i++;
    }
}

void empty_ai_cmd_queue(client_t *client, const char *team,
    server_info_t *info, clients_management_t *client_manage)
{
    for (int i = 0; info->names[i] != NULL; ++i) {
        if (strcmp(team, info->names[i]) == 0) {
            handle_ai_client(info, client_manage, client);
            break;
        }
    }
}

void manage_client_input(server_info_t *info,
    clients_management_t *client_manage, int client_index)
{
    client_t *client = &client_manage->clients[client_index];
    const char *team = client_manage->clients[client_index].team;
    int removed = 0;

    read_client_input(client_manage, client_index, &removed);
    if (removed)
        return;
    if (strcmp(team, "NOTEAM") == 0){
        handle_unassigned_client(info, client_manage, client);
        return;
    }
    if (strcmp(team, "GRAPHIC") == 0){
        empty_gui_cmd_queue(client, info);
        return;
    }
    while (strlen(client->buffer) > 0)
        empty_ai_cmd_queue(client, team, info, client_manage);
    return;
}
