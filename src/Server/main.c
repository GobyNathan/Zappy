/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** main for zappy
*/

#include "Server/server_info.h"
#include "Server/server.h"
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

/**
* @brief main function, handles the core of the program.
* @param argc
* @param argv
* @return int
*/
int main(int argc, char **argv)
{
    server_info_t *info = NULL;

    srand(time(NULL));
    if (robustness_check(argc) == 84) {
        return 84;
    }
    info = parse_server_info(argc, argv);
    print_server_info(info);
    run_server(info);
    return 0;
}
