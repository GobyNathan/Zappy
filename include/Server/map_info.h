/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server
*/

#ifndef MAP_INFO_H
    #include "mineral_info.h"
    #define MAP_INFO_H
    #define RESOURCE_NUM 7
    #define FOOD_DENSITY 0.5
    #define LINEMATE_DENSITY 0.3
    #define DERAUMERE_DENSITY 0.15
    #define SIBUR_DENSITY 0.1
    #define MENDIANE_DENSITY 0.1
    #define PHIRAS_DENSITY 0.08
    #define THYSTAME_DENSITY 0.05
    #include "Server/action_queue_info.h"
    #include <sys/time.h>

enum ELEMENT {
    FOOD = 1,
    MINERAL = 2,
    INDIVIDUAL = 3
};

typedef struct element_a {
    enum ELEMENT elem_type;
    void *elem;
    mineral_t mineral_type;
    struct element_a *next;
} element_t;

typedef struct resource_template_a {
    char *name;
    float density;
} resource_template_t;

static const resource_template_t resource_templates[] = {
    {"food", FOOD_DENSITY},
    {"linemate", LINEMATE_DENSITY},
    {"deraumere", DERAUMERE_DENSITY},
    {"sibur", SIBUR_DENSITY},
    {"mendiane", MENDIANE_DENSITY},
    {"phiras", PHIRAS_DENSITY},
    {"thystame", THYSTAME_DENSITY},
};


typedef struct map_info_a {
    struct element_a ***map_array;
    int size_x;
    int size_y;
    int *current_tick;
    scheduled_action_t **action_queue;
    element_t *element_queue;
} map_info_t;

#endif /* MAP_INFO_H */
