/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** client_utilities.c
*/

#include "Server/utilities.h"
#include "Server/gui_protocol.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/poll.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void add_poll_fd(clients_management_t *client_manage, struct pollfd *fd)
{
    int new_size = sizeof(struct pollfd) * (client_manage->len_fds + 1);
    struct pollfd *newFds = realloc(client_manage->fds, new_size);

    if (newFds == NULL){
        perror("Error when realloc pollfds");
        return;
    }
    client_manage->fds = newFds;
    client_manage->len_fds++;
    client_manage->fds[client_manage->len_fds - 1] = *fd;
}

static void add_client(clients_management_t *client_manage, int fd)
{
    client_t newClient;
    int new_size = sizeof(client_t) * (client_manage->len_clients + 1);
    client_t *newClients = realloc(client_manage->clients, new_size);

    if (newClients == NULL){
        perror("Error when realloc clients array");
        return;
    }
    client_manage->clients = newClients;
    newClient.individual = NULL;
    newClient.sockfd = fd;
    newClient.id = client_manage->all_time_clients;
    newClient.team = strdup("NOTEAM");
    newClient.buffer = malloc(sizeof(char) * CLIENT_BUFFER_SIZE);
    memset(newClient.buffer, 0, CLIENT_BUFFER_SIZE);
    client_manage->len_clients++;
    client_manage->all_time_clients++;
    client_manage->clients[client_manage->len_clients - 1] = newClient;
}

void accept_client(clients_management_t *client_manage)
{
    int client_sockfd = -1;
    struct sockaddr_in addr;
    socklen_t len_addr = sizeof(addr);
    int serv_fd = client_manage->fds[0].fd;
    struct pollfd clientPollfd;

    client_sockfd = accept(serv_fd, (struct sockaddr *)&addr, &len_addr);
    clientPollfd = init_poll_fd(client_sockfd, POLLIN);
    if (client_sockfd >= 0){
        add_client(client_manage, client_sockfd);
        add_poll_fd(client_manage, &clientPollfd);
        write(client_sockfd, "WELCOME\n", 8);
    }
}

void remove_client(clients_management_t *client_manage, int client_index)
{
    client_t client = client_manage->clients[client_index];

    if (client.individual != NULL){
        client.individual->is_alive = false;
        send_pdi_all(client.individual, client_manage);
    }
    close(client.sockfd);
    free(client.team);
    free(client.buffer);
    erase_client(client_manage, client_index);
    erase_fd(client_manage, client_index + 1);
}

void pop_command(client_t *client)
{
    char *command = substring_until(client->buffer, '\n');
    size_t offset = strlen(command) + 1;
    size_t remaining = strlen(client->buffer + offset);

    memmove(client->buffer, client->buffer + offset, remaining + 1);
    free(command);
}

static ssize_t write_all(int fd, const void *msg, size_t count)
{
    size_t total_written = 0;
    ssize_t written = 0;
    const char *ptr = msg;

    while (total_written < count) {
        written = write(fd, ptr + total_written,
            count - total_written);
        if (written <= 0)
            return -1;
        total_written += written;
    }
    return total_written;
}

void send_msg_client(client_t *client, const char *msg)
{
    struct pollfd p = { client->sockfd, POLLOUT, 0 };

    if (poll(&p, 1, 500) > 0 && (p.revents & POLLOUT)) {
        write_all(client->sockfd, msg, strlen(msg));
    }
}

client_t *get_client_by_id(int id, clients_management_t *manage)
{
    for (int i = 0; i < manage->len_clients; ++i){
        if (manage->clients[i].id == id){
            return &manage->clients[i];
        }
    }
    return NULL;
}
