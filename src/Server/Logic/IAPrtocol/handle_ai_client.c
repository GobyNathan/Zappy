/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_ai_client.c
*/
#include "Server/ai_protocol_info.h"
#include "Server/server.h"
#include "Server/server_info.h"
#include "Server/utilities.h"
#include "Server/ia_messages.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *join_strings(char **strings, const char *sep)
{
    size_t total_len = 0;
    size_t sep_len = strlen(sep);
    char *result = NULL;

    for (int i = 0; strings[i] != NULL; i++) {
        total_len += strlen(strings[i]);
        if (strings[i + 1])
            total_len += sep_len;
    }
    result = malloc(total_len + 1);
    result[0] = '\0';
    for (int i = 0; strings[i] != NULL; i++) {
        strcat(result, strings[i]);
        if (strings[i + 1])
            strcat(result, sep);
    }
    return result;
}

char **split_string(char *input, const char *delim)
{
    char **ret = NULL;
    char *token = strtok(input, delim);
    int found = 0;

    while (token) {
        ret = realloc(ret, sizeof(char *) * (found + 2));
        ret[found] = strdup(token);
        found++;
        token = strtok(NULL, delim);
    }
    if (ret)
        ret[found] = NULL;
    return ret;
}

static ai_command_args_t *init_args(server_info_t *info,
    clients_management_t *client_manage, char **command_args)
{
    ai_command_args_t *args = malloc(sizeof(ai_command_args_t));

    if (!args)
        return NULL;
    args->map = info->map;
    args->ptr_individuals = &info->individuals;
    args->command_args = command_args;
    args->client_manage = client_manage;
    return args;
}

void handle_ai_command(char *command_with_args,
    clients_management_t *client_manage, server_info_t *info, client_t *client)
{
    ai_command_info_t cmd = {0};
    ai_command_args_t *args = init_args(info, client_manage,
        split_string(command_with_args, " "));

    if (!args || !args->command_args || !args->command_args[0]){
        send_msg_client(client, MSG_KO);
        return;
    }
    for (int i = 0; i != AI_COMMAND_NB; i++) {
        cmd = AI_COMMANDS[i];
        if (strcmp(args->command_args[0], cmd.name) != 0)
            continue;
        cmd.callback(
            client,
            args,
            cmd.delay_factor
        );
        return;
    }
    send_msg_client(client, MSG_KO);
}

void handle_ai_client(server_info_t *info, clients_management_t *client_manage,
    client_t *client)
{
    char *command_with_args = substring_until(client->buffer, '\n');
    char *command = NULL;

    if (!command_with_args)
        return;
    command = substring_until(command_with_args, ' ');
    handle_ai_command(command_with_args, client_manage, info, client);
    free(command);
    free(command_with_args);
    pop_command(client);
}
