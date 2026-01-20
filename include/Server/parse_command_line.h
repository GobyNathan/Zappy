/*
** EPITECH PROJECT, 2025
** parse_command_line.h
** File description:
** utility to parse the command line
*/

#ifndef PARSE_COMMAND_LINE_H
    #define PARSE_COMMAND_LINE_H
    #include "server_info.h"

server_info_t parse_command_line(int argc, char **argv);
void print_conn_info(server_info_t *info);

#endif /* PARSE_COMMAND_LINE_H */
