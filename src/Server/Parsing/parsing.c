/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** parsing.c
*/

#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/**
* @brief parses integer and stock it at the adress pointed by out.
* @param str string containing a number
* @param out pointer of the int that we want to stock the parsed value in.
* @return int 0 on success, -1 on fail
*/
int parse_integer(const char *arg, int *out)
{
    char *endptr = NULL;
    long val = strtol(arg, &endptr, 10);

    if (endptr == arg || *endptr != '\0') {
        fprintf(stderr, "Invalid number: %s\n", arg);
        return -1;
    }
    if (val < INT_MIN || val > INT_MAX) {
        fprintf(stderr, "Number out of range: %s\n", arg);
        return -1;
    }
    *out = (int)val;
    return 0;
}
