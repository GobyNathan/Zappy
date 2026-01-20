/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** utilities
*/

#ifndef INCLUDED_UTILITIES_H
    #define INCLUDED_UTILITIES_H
    #define MAX_LEVEL 8
    #include "Server/server_info.h"
    #include <poll.h>

typedef struct elevation_req_s {
    int players;
    int stones[6];
} elevation_req_t;

static const elevation_req_t ELEVATION_REQUIREMENTS[] = {
    {1, {1, 0, 0, 0, 0, 0}},
    {2, {1, 1, 1, 0, 0, 0}},
    {2, {2, 0, 1, 0, 2, 0}},
    {4, {1, 1, 2, 0, 1, 0}},
    {4, {1, 2, 1, 3, 0, 0}},
    {6, {1, 2, 3, 0, 1, 0}},
    {6, {2, 2, 2, 2, 2, 1}},
};

void erase_client(clients_management_t *client_manage, int index_erase);
void erase_fd(clients_management_t *client_manage, int index_erase);
void remove_client(clients_management_t *client_manage, int client_index);
int create_server_socket(int port);
struct pollfd init_poll_fd(int fd, short events);
void accept_client(clients_management_t *);
char *substring_until(const char *str, char delimiter);
void pop_command(client_t *client);
void send_msg_client(client_t *client, const char *msg);
char *join_strings(char **strings, const char *sep);
const char *get_mineral_name_from_type(mineral_t type);
client_t *get_client_by_id(int id, clients_management_t *manage);

#endif
