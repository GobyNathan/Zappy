/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** string_until.c
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *substring_until(const char *str, char delimiter)
{
    const char *pos = NULL;
    size_t len = 0;
    char *result = NULL;

    if (!str)
        return NULL;
    pos = strchr(str, delimiter);
    len = pos != NULL ? (size_t)(pos - str) : strlen(str);
    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return NULL;
    strncpy(result, str, len);
    result[len] = '\0';
    return result;
}
