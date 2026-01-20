/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#ifndef INCLUDED_SERVER_H
    #define INCLUDED_SERVER_H
    #include "Server/server_info.h"
    #include <poll.h>

void server_help(void);
int robustness_check(int argc);
void print_server_info(server_info_t *info);

server_info_t *parse_server_info(int argc, char **argv);
int parse_integer(const char *arg, int *out);

void run_server(server_info_t *info);
void manage_client_input(server_info_t *, clients_management_t *, int);

void send_msg_graphics(clients_management_t *client_manage, char *msg);

void handle_ai_client(server_info_t *info, clients_management_t *client_manage,
    client_t *client);

void decrease_food_players(server_info_t *info,
    clients_management_t *client_manage);
void remove_dead_players(clients_management_t *client_manage);
void endgame(server_info_t *info, clients_management_t *manage);
bool is_endgame(server_info_t *info);

#endif
