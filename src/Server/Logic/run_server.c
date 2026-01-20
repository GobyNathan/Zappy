/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** run_server.c
*/

#include "Server/server.h"
#include "Server/server_info.h"
#include "Server/utilities.h"
#include "Server/map.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "string.h"
#include <sys/time.h>
#include <Server/gui_protocol.h>
#include "Server/action_queue.h"

static void server_loop(clients_management_t *client_manage,
    server_info_t *info)
{
    poll(client_manage->fds, client_manage->len_fds, 10);
    if (client_manage->fds[0].revents & POLLIN)
        accept_client(client_manage);
    for (int i = 1; i < client_manage->len_fds; ++i){
        if (client_manage->fds[i].revents & POLLIN)
            manage_client_input(info, client_manage, i - 1);
    }
}

static void setup_clients(clients_management_t *client_manage, int server_fd)
{
    client_manage->clients = NULL;
    client_manage->len_clients = 0;
    client_manage->len_fds = 1;
    client_manage->all_time_clients = 0;
    client_manage->fds = malloc(sizeof(struct pollfd));
    if (!client_manage->fds)
        return;
    client_manage->fds[0] = init_poll_fd(server_fd, POLLIN);
}

void handle_tick(server_info_t *info, clients_management_t *manage,
    double *accumulator)
{
    while (*accumulator >= (1.0 / info->freq)) {
        tick_action_queue(&info->action_queue, info->current_tick);
        info->current_tick++;
        if (info->current_tick % 126 == 0)
            decrease_food_players(info, manage);
        if (info->current_tick % 20 == 0)
            use_element_queue(info->map, &info->map->element_queue, manage);
        *accumulator -= (1.0 / info->freq);
    }
}

void run_server(server_info_t *info)
{
    int server_fd = create_server_socket(info->port);
    clients_management_t client_manage;
    struct timeval last_time;
    struct timeval now;
    double elapsed = 0;
    double accumulator = 0;

    gettimeofday(&last_time, NULL);
    setup_clients(&client_manage, server_fd);
    while (1){
        if (is_endgame(info))
            endgame(info, &client_manage);
        gettimeofday(&now, NULL);
        elapsed = (now.tv_sec - last_time.tv_sec) +
            (now.tv_usec - last_time.tv_usec) / 1e6;
        handle_tick(info, &client_manage, &accumulator);
        last_time = now;
        accumulator += elapsed;
        server_loop(&client_manage, info);
    }
}
