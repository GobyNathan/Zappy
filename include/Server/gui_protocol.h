/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** gui_protocol
*/

#ifndef INCLUDED_GUI_PROTOCOL_H
    #define INCLUDED_GUI_PROTOCOL_H
    #include "Server/ai_protocol_info.h"
    #include "server_info.h"

void msz_command(server_info_t *info, client_t *client);
void bct_command(server_info_t *info, client_t *client, char *command);
void send_bct_all(int x, int y, map_info_t *map,
    clients_management_t *client_manage);
void mct_command(server_info_t *info, client_t *client);
void tna_command(server_info_t *info, client_t *client);
void ppo_command(server_info_t *info, client_t *client, char *command);
void send_ppo_all(individual_t *player, clients_management_t *client_manage);
void plv_command(server_info_t *info, client_t *client, char *command);
void send_plv_all(individual_t *player, clients_management_t *client_manage);
void pin_command(server_info_t *info, client_t *client, char *command);
void send_pin_all(individual_t *player, clients_management_t *client_manage);
void sgt_command(server_info_t *info, client_t *client);
void sst_command(server_info_t *info, client_t *client, char *command);
void send_pnw(individual_t *individual, client_t *client);
void send_pnw_all(individual_t *individual,
    clients_management_t *client_manage);
void send_ebo(individual_t *egg, client_t *client);
void send_ebo_all(individual_t *egg,
    clients_management_t *client_manage);
void send_pdi(individual_t *individual, client_t *client);
void send_pdi_all(individual_t *individual,
    clients_management_t *client_manage);
void send_enw(individual_t *individual, client_t *client, int forker_id);
void send_enw_all(individual_t *individual,
    clients_management_t *client_manage, int forker_id);
void send_pfk_all(individual_t *individual,
    clients_management_t *client_manage);
void send_pic_all(int *group, int group_size, individual_t *launcher,
    clients_management_t *manage);
void send_pie_all(int posX, int posY, bool success,
    clients_management_t *manage);
void handle_gui_client(server_info_t *info, client_t *client);
void new_gui_client(server_info_t *info, client_t *client);
void send_pex_all(individual_t *launcher, clients_management_t *manage);
void send_pbc_all(individual_t *launcher, const char *content,
    clients_management_t *manage);
void send_pdr_all(individual_t *launcher, const char *resource,
    clients_management_t *manage);
void send_pgt_all(individual_t *launcher, const char *resource,
    clients_management_t *manage);
void send_edi_all(individual_t *egg,
    clients_management_t *client_manage);
void send_seg_all(char *team, clients_management_t *manage);

#endif
