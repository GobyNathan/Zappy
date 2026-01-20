/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** robustness.c
*/

#include "Server/server.h"

/**
* @brief basic robustness checks for server arguments.
* @param argc
* @return 0 if everything is ok, 84 otherwise.
*/
int robustness_check(int argc)
{
    if (argc < 13) {
        server_help();
        return 84;
    }
    return 0;
}
