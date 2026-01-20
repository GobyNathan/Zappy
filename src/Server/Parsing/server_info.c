/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** server_info_parsing.c
*/

#include "Server/server.h"
#include "Server/server_info.h"
#include "Server/map.h"
#include "Server/individual.h"
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
* @brief parses all names into a singular 2d array
* @param argc
* @param argv
* @return char**
*/
static char **parse_team_names(char **argv)
{
    int name_count = 0;
    int n = 0;
    char **names = NULL;

    for (int i = optind - 1; argv[i] && argv[i][0] != '-'; i++) {
        name_count++;
    }
    names = malloc(sizeof(char *) * (name_count + 1));
    if (!names) {
        return (char *[]){"ERROR_ALLOC"};
    }
    names[name_count] = NULL;
    for (int i = optind - 1; argv[i] && argv[i][0] != '-'; i++) {
        names[n] = strdup(argv[i]);
        n++;
    }
    optind += name_count - 1;
    return names;
}

/**
* @brief parses a singular argument and inserts it into the info structure
* @param info
* @param opt
*/
static void parse_args_beta(server_info_t *info, int opt, int *map_coord)
{
    switch (opt) {
        case 'p':
            if (parse_integer(optarg, &info->port) != 0)
                exit(84);
            break;
        case 'x':
            if (parse_integer(optarg, &map_coord[0]) != 0)
                exit(84);
            break;
        case 'y':
            if (parse_integer(optarg, &map_coord[1]) != 0)
                exit(84);
            break;
        case 'h':
            server_help();
            exit(0);
        case '?':
            exit(84);
    }
}

/**
* @brief parses a singular argument and inserts it into the info structure
* @param info
* @param opt
*/
static void parse_args_alpha(server_info_t *info, char **argv, int opt,
    int *map_coord)
{
    switch (opt) {
        case 'n':
            info->names = parse_team_names(argv);
            break;
        case 'c':
            if (parse_integer(optarg, &info->original_slots_nb) != 0)
                exit(84);
            break;
        case 'f':
            if (parse_integer(optarg, &info->freq) != 0)
                exit(84);
            break;
    }
    parse_args_beta(info, opt, map_coord);
}

static int get_nb_teams(server_info_t *info)
{
    int size = 0;

    for (int i = 0; info->names[i] != NULL; ++i){
        size++;
    }
    return size;
}

static void init_individuals(server_info_t *info)
{
    int nb_teams = get_nb_teams(info);
    int nb_individuals = nb_teams * info->original_slots_nb;
    int k = 0;

    info->individuals = malloc(sizeof(individual_t *) * (nb_individuals + 1));
    info->individuals[nb_individuals] = NULL;
    for (int i = 0; i < nb_teams; ++i) {
        for (int j = 0; j < info->original_slots_nb; ++j) {
            info->individuals[k] = create_individual(k, info->names[i],
                rand() % info->map->size_x, rand() % info->map->size_y);
            k++;
        }
    }
    info->individuals[k] = NULL;
}

/**
* @brief debug print of the struct server_info_t
*
* @param info
*/
void print_server_info(server_info_t *info)
{
    printf("Port: %d\n", info->port);
    printf("Map Size: %d x %d\n", info->map->size_x, info->map->size_y);
    printf("Clients per team: %d\n", info->original_slots_nb);
    printf("Frequency: %d\n", info->freq);
    printf("Team Names:\n");
    if (info->names) {
        for (int i = 0; info->names[i] != NULL; i++) {
            printf("  - %s\n", info->names[i]);
        }
    } else {
        printf("  (None)\n");
    }
}

/**
* @brief parses and return command line args.
* @param argc argc
* @param argv argv
* @return conn_info_t with parsed information.
*/
server_info_t *parse_server_info(int argc, char **argv)
{
    server_info_t *info = malloc(sizeof(server_info_t));
    int map_coord[2] = {0, 0};
    int opt = getopt(argc, argv, "p:x:y:n:c:f:h");

    while (opt != -1) {
        parse_args_alpha(info, argv, opt, map_coord);
        opt = getopt(argc, argv, "p:x:y:n:c:f:h");
    }
    info->current_tick = 0;
    info->action_queue = NULL;
    info->individuals = NULL;
    info->map = init_map(map_coord[0], map_coord[1],
        &info->current_tick, &info->action_queue);
    init_individuals(info);
    return info;
}
