/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** new_gui_client.c
*/

#include "Server/server_info.h"
#include "Server/gui_protocol.h"
#include <stddef.h>
#include <string.h>

void new_gui_client(server_info_t *info, client_t *client)
{
    client->team = strdup("GRAPHIC");
    msz_command(info, client);
    sgt_command(info, client);
    mct_command(info, client);
    tna_command(info, client);
    for (int i = 0; info->individuals[i] != NULL; ++i){
        if (info->individuals[i]->is_egg)
            send_enw(info->individuals[i], client, -1);
        if (info->individuals[i]->is_alive)
            send_pnw(info->individuals[i], client);
    }
}
