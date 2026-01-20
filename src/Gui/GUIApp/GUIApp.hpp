/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Jun, 2025                                                     *
 * Title           - Zappy                                                              *
 * Description     -                                                                    *
 *     GUIApp                                                                           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once
#include <iostream>

#include "../map/Map.hpp"
#include "../hud/Hud.hpp"
#include "../Protocol/Protocol.hpp"
#include "../BroadcastCircle/BroadcastCircle.hpp"
#include <memory>
#include <algorithm> 
#include <cctype> 

#include <raymath.h>
#include <raylib.h>


enum CAM_MODE { TOP, FREE, LAST_ELM, PLAYER_POV, WINING_POV };

class GuiApp {
    public:
        GuiApp(int windowWidth, int windowHeight, std::string host, int port, const char* title);

        ~GuiApp();

        void run();

    private:
        int _windowWidth, _windowHeight;
        float _mapWidth, _mapHeight;
        Texture2D _background;
        Camera3D _camera;
        Hud _hud;
        Map _map;
        Protocol _protocol;

        CAM_MODE _cam_mode;
        bool _block_view;

        int _tickPerSecond;

        std::map<std::string, std::function<void(const std::vector<std::string>&)>> _handlers;
        std::string _lastResponse = "caca";

        std::unordered_map<std::string, Color> _teamColors;

        std::vector<std::string> _stoneNameList = {
            "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
        };

        BroadcastCircles _broadcastCircles;

        bool _followingPlayer = false;
        int _followedPlayerId = -1;
        Rectangle _povButton = {0, 0, 0, 0};

        Player *_selectedPlayer;
        Egg *_selectedEgg;
        
        Music _music;
        std::pair<bool, std::string> _drawWin = {false, ""};

        void AskServerForUpdate();

        void handleEvents();
        void initHandlers();
        void handleCommand(const std::string &reponse);
        std::vector<std::string> splitLine(const std::string &line);

        void handleMsz(const std::vector<std::string>& cmd);
        void handleBct(const std::vector<std::string>& cmd);
        void handleTna(const std::vector<std::string>& cmd);
        void handlePnw(const std::vector<std::string>& cmd);
        void handlePpo(const std::vector<std::string>& cmd);
        void handlePlv(const std::vector<std::string>& cmd);
        void handlePin(const std::vector<std::string>& cmd);
        void handlePex(const std::vector<std::string>& cmd);
        void handlePbc(const std::vector<std::string>& cmd);
        void handlePic(const std::vector<std::string>& cmd);
        void handlePie(const std::vector<std::string>& cmd);
        void handlePfk(const std::vector<std::string>& cmd);
        void handlePdr(const std::vector<std::string>& cmd);
        void handlePgt(const std::vector<std::string>& cmd);
        void handlePdi(const std::vector<std::string>& cmd);
        void handleEnw(const std::vector<std::string>& cmd);
        void handleEbo(const std::vector<std::string>& cmd);
        void handleEdi(const std::vector<std::string>& cmd);
        void handleSgt(const std::vector<std::string>& cmd);
        void handleSst(const std::vector<std::string>& cmd);
        void handleSeg(const std::vector<std::string>& cmd);
        void handleSmg(const std::vector<std::string>& cmd);
        void handleSuc(const std::vector<std::string>& cmd);
        void handleSbp(const std::vector<std::string>& cmd);

        void winingViewMode(int playerId);
        void render();

        void changeViewMode();

        void updatePlayerPOV();
        void drawButton(Rectangle bounds, const char* text, bool isToggled);
};
