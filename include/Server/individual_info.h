/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/


#ifndef INDIVIDUAL_INFO_H
    #define INDIVIDUAL_INFO_H
    #define DEFAULT_VISION 1
    #include <stdbool.h>

typedef enum lookat_s {
    NORTH,
    EAST,
    SOUTH,
    WEST
} lookat_t;

typedef struct look_at_info_a {
    int fy;
    int fx;
    int ly;
    int lx;
} look_at_info_t;

typedef struct inventory_a {
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

typedef struct individual_a {
    int player_id;
    int food_lvl;
    int main_lvl;
    int vision_range;
    bool is_alive;
    int *elevation_group;
    int elevation_group_size;
    bool is_egg;
    char *to_broadcast;
    lookat_t look_at;
    int pos_x;
    int pos_y;
    inventory_t *inventory;
    char *team;
} individual_t;

#endif /* INDIVIDUAL_INFO_H */
