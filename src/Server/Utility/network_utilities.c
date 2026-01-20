/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** network_utilities.c
*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <asm-generic/socket.h>

static struct sockaddr_in init_server_adress(int port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}

int create_server_socket(int port)
{
    struct sockaddr_in addr = init_server_adress(port);
    socklen_t len_addr = sizeof(addr);
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    int opt_name = SO_REUSEADDR | SO_REUSEPORT;

    if (server_sockfd == 0){
        perror("bind failed");
        return -1;
    }
    setsockopt(server_sockfd, SOL_SOCKET, opt_name, &opt, sizeof(opt));
    if (bind(server_sockfd, (struct sockaddr *)&addr, len_addr) < 0){
        perror("bind failed");
        return -1;
    }
    if (listen(server_sockfd, 1000) < 0){
        perror("listen failed");
        return -1;
    }
    return server_sockfd;
}

struct pollfd init_poll_fd(int fd, short events)
{
    struct pollfd result;

    result.fd = fd;
    result.events = events;
    result.revents = 0;
    return result;
}
