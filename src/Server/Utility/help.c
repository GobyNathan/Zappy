/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** help.c
*/

#include <stdio.h>

/**
* @brief prints help text for server program.
*/
void server_help(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 [...other_teams_names] -c clientsNb -f freq\n");
}
