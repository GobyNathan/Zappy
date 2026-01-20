/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** sst.c
*/

#include "Server/gui_protocol.h"
#include "Server/utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Server/messages.h"

static int get_sst_new_freq(char *command)
{
    char *command_cpy = strdup(command);
    char *f_text = NULL;
    int f = 0;
    char *endptr = NULL;

    strtok(command_cpy, " \t");
    f_text = strtok(NULL, " \t");
    if (f_text == NULL){
        free(command_cpy);
        return -1;
    }
    f = (int) strtol(f_text, &endptr, 10);
    if (*endptr != '\0'){
        free(command_cpy);
        return -1;
    }
    free(command_cpy);
    return f;
}

void sst_command(server_info_t *info, client_t *client, char *command)
{
    char response[1024] = {0};
    int new_f = get_sst_new_freq(command);

    if (new_f < 0){
        send_msg_client(client, "sbp\n");
        return;
    }
    info->freq = new_f;
    snprintf(response, sizeof(response), MSG_SST, info->freq);
    send_msg_client(client, response);
}
