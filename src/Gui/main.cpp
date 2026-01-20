/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Jun, 2025                                                     *
 * Title           - Zappy                                                              *
 * Description     -                                                                    *
 *     main                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

 #include "GUIApp/GUIApp.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

void launch_GUI(const std::string& host, int port)
{
    GuiApp app(WIN_WIDTH, WIN_HEIGHT, host, port, "Zappy");
    app.run();
}

int main(int ac, char** av)
{
    srand(time(nullptr));
    int port = 0;
    std::string host = "localhost";

    int opt;
    while ((opt = getopt(ac, av, "p:h:")) != -1) {
        switch (opt) {
            case 'p':
                port = std::atoi(optarg);
                break;
            case 'h':
                host = optarg;
                break;
            default:
                std::cerr << "Usage: " << av[0] << " -p port -h host" << std::endl;
                return 84;
        }
    }

    if (port == 0) {
        std::cerr << "Port is required (-p)" << std::endl;
        return 84;
    }

    launch_GUI(host, port);
    return 0;
}