/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** elevation_group.c
*/

#include "Server/utilities.h"
#include <stddef.h>
#include <stdlib.h>

static bool player_on_tile(int player_id, int posX, int posY,
    individual_t **all)
{
    individual_t *p = NULL;

    for (int i = 0; all[i] != NULL; ++i){
        if (all[i]->player_id == player_id){
            p = all[i];
            break;
        }
    }
    if (!p->is_alive)
        return false;
    if (p->pos_x != posX || p->pos_y != posY)
        return false;
    return true;
}

static bool is_still_elevating(int player_id, individual_t **all)
{
    for (int i = 0; all[i] != NULL; ++i){
        if (all[i]->player_id == player_id){
            return all[i]->elevation_group != NULL;
        }
    }
    return false;
}

bool in_group(int *group, int group_size, int player_id)
{
    for (int i = 0; i < group_size; ++i){
        if (group[i] == player_id)
            return true;
    }
    return false;
}

static bool is_elevation_mate(individual_t *p, individual_t *mate)
{
    if (mate->elevation_group != NULL || !mate->is_alive ||
        mate->main_lvl != p->main_lvl || mate->pos_x != p->pos_x ||
        mate->pos_y != p->pos_y)
        return false;
    return true;
}

static int get_group_size(individual_t *indiv, individual_t **all)
{
    int count = 0;
    individual_t *p = NULL;

    for (int i = 0; all[i] != NULL; i++){
        p = all[i];
        if (!is_elevation_mate(indiv, p))
            continue;
        count++;
    }
    return count;
}

int *get_elevation_group(individual_t *indiv, individual_t **all)
{
    int group_size = get_group_size(indiv, all);
    int req_size = ELEVATION_REQUIREMENTS[indiv->main_lvl - 1].players;
    individual_t *p = NULL;
    int *elevation_group = NULL;
    int count = 0;

    if (group_size < req_size)
        return NULL;
    elevation_group = malloc(sizeof(int) * group_size);
    for (int i = 0; all[i] != NULL; i++){
        p = all[i];
        if (!is_elevation_mate(indiv, p))
            continue;
        elevation_group[count] = p->player_id;
        count++;
    }
    indiv->elevation_group_size = group_size;
    indiv->elevation_group = elevation_group;
    return elevation_group;
}

bool last_to_elevate(int *elevation_group, int group_size,
    individual_t **all, individual_t *individual)
{
    for (int i = 0; i < group_size; ++i){
        if (is_still_elevating(elevation_group[i], all) &&
        elevation_group[i] != individual->player_id)
            return false;
    }
    return true;
}

bool elevation_group_stayed(int *elevation_group, int group_size,
    individual_t **all, individual_t *individual)
{
    int posX = individual->pos_x;
    int posY = individual->pos_y;

    for (int i = 0; i < group_size; ++i){
        if (!player_on_tile(elevation_group[i], posX, posY, all))
            return false;
    }
    return true;
}
