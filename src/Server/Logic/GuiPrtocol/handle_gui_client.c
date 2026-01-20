/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** handle_gui_client.c
*/
#include "Server/gui_protocol.h"
#include "Server/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void handle_gui_command(char *command_with_args, char *command,
    server_info_t *info, client_t *client)
{
    if (strcmp(command, "msz") == 0)
        return msz_command(info, client);
    if (strcmp(command, "bct") == 0)
        return bct_command(info, client, command_with_args);
    if (strcmp(command, "mct") == 0)
        return mct_command(info, client);
    if (strcmp(command, "tna") == 0)
        return tna_command(info, client);
    if (strcmp(command, "ppo") == 0)
        return ppo_command(info, client, command_with_args);
    if (strcmp(command, "plv") == 0)
        return plv_command(info, client, command_with_args);
    if (strcmp(command, "pin") == 0)
        return pin_command(info, client, command_with_args);
    if (strcmp(command, "sgt") == 0)
        return sgt_command(info, client);
    if (strcmp(command, "sst") == 0)
        return sst_command(info, client, command_with_args);
    send_msg_client(client, "suc\n");
}

void handle_gui_client(server_info_t *info, client_t *client)
{
    char *command_with_args = substring_until(client->buffer, '\n');
    char *command = substring_until(command_with_args, ' ');

    if (strlen(command) == strlen(client->buffer)){
        free(command);
        free(command_with_args);
        return;
    }
    handle_gui_command(command_with_args, command, info, client);
    pop_command(client);
    free(command);
    free(command_with_args);
}
