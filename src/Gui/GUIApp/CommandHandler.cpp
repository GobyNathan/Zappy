/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Jun, 2025                                                     *
 * Title           - mirrorZappy                                                        *
 * Description     -                                                                    *
 *     CommandHandler                                                                   *
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

#include "GUIApp.hpp"

void GuiApp::handleMsz(const std::vector<std::string>& cmd)
{
    _mapWidth = atof(cmd[1].c_str());
    _mapHeight = atof(cmd[2].c_str());

    _map.initMapSize(_mapWidth, _mapHeight);
    _broadcastCircles.setMaxRadius(std::fmax(_mapWidth, _mapHeight));
}

void GuiApp::handleBct(const std::vector<std::string>& cmd)
{
    int tileX = atoi(cmd[1].c_str());
    int tileY = atoi(cmd[2].c_str());

    std::vector<int> quantities;
    quantities.reserve(7);
    quantities.push_back(atoi(cmd[3].c_str())); // food
    quantities.push_back(atoi(cmd[4].c_str())); // linemate
    quantities.push_back(atoi(cmd[5].c_str())); // deraumere
    quantities.push_back(atoi(cmd[6].c_str())); // sibur
    quantities.push_back(atoi(cmd[7].c_str())); // mendiane
    quantities.push_back(atoi(cmd[8].c_str())); // phiras
    quantities.push_back(atoi(cmd[9].c_str())); // thystame

    std::vector<int> quantityDifferences;
    quantityDifferences.reserve(7);
    quantityDifferences.push_back(quantities[0] - _map.countFood(tileX, tileY));
    for (std::size_t i = 1; i < quantities.size(); i++) {
        quantityDifferences.push_back(quantities[i] - _map.countStone(tileX, tileY, _stoneNameList[i-1]));
    }

    // j'ajoute ou retire de la nourriture sur la tile si nécessaire
    if (quantityDifferences[0] > 0) {
        for (int i = 0; i < quantityDifferences[0]; i++) {
            _map.addFood(tileX, tileY);
        }
    } else if (quantityDifferences[0] < 0) {
        for (int i = 0; i < -quantityDifferences[0]; i++) {
            _map.delFood(tileX, tileY);
        }
    }
    
    // j'ajoute ou retire des pierres sur la tile si nécessaire
    for (std::size_t i = 1; i < quantityDifferences.size(); i++) {
        if (quantityDifferences[i] > 0) {
            for (int j = 0; j < quantityDifferences[i]; j++) {
                _map.addStone(tileX, tileY, _stoneNameList[i-1]);
            }
        } else if (quantityDifferences[i] < 0) {
            for (int j = 0; j < -quantityDifferences[i]; j++) {
                _map.delStone(tileX, tileY, _stoneNameList[i-1]);
            }
        }
    }
}

void GuiApp::handleTna(const std::vector<std::string>& cmd)
{
    unsigned char r = GetRandomValue(0, 255);
    unsigned char g = GetRandomValue(0, 255);
    unsigned char b = GetRandomValue(0, 255);
    unsigned char a = GetRandomValue(125, 200);
    _teamColors.insert({cmd[1], (Color){r, g, b, a}});
}

void GuiApp::handlePnw(const std::vector<std::string>& cmd)
{
    std::cout << "handle pnw\n";
    int id = std::atoi(cmd[1].c_str() + 1);
    float rotation = (std::atoi(cmd[4].c_str()) * 90) % 360;
    std::string teamName = cmd[6];

    try {
        int posX = std::atoi(cmd[2].c_str());
        int posY = std::atoi(cmd[3].c_str());

        // _map.delEggAtPos(posX, posY);
        _map.addPlayer(posX, posY, id, rotation, teamName, _teamColors[teamName]);
        _map.getPlayerAtId(id).setLevel(std::atoi(cmd[5].c_str()));
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't handle command pnw, no egg found with ID: " << id << std::endl;
    }
}

void GuiApp::handlePpo(const std::vector<std::string>& cmd)
{
    _map.setPlayerPos(std::atoi(cmd[1].c_str() + 1), std::atoi(cmd[2].c_str()), std::atoi(cmd[3].c_str()), std::atof(cmd[4].c_str()));
}

void GuiApp::handlePlv(const std::vector<std::string>& cmd)
{
    _map.setPlayerLevel(atoi(cmd[2].c_str()), atoi(cmd[1].c_str() + 1));
}

void GuiApp::handlePin(const std::vector<std::string>& cmd)
{
    _map.setPlayerInventory(std::atoi(cmd[1].c_str() + 1), std::atoi(cmd[2].c_str()), std::atoi(cmd[3].c_str()), std::atoi(cmd[4].c_str()), std::atoi(cmd[5].c_str()), std::atoi(cmd[6].c_str()), std::atoi(cmd[7].c_str()), std::atoi(cmd[8].c_str()));
}

void GuiApp::handlePex(const std::vector<std::string>& cmd)
{
    std::cout << cmd[1] << "is expulsing everyone from his tile" << std::endl;
}

void GuiApp::handlePbc(const std::vector<std::string>& cmd)
{
    if (cmd.size() <= 2)
        return;

    try {
        Player &player = _map.getPlayerAtId(atoi(cmd[1].c_str() + 1));
        player.launchBroadcast(cmd[2]);
        Vector3 pos = player.getPosVec();
        _broadcastCircles.addCircle(std::make_unique<Circle>(pos));
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't handle command pbc, no player found with ID: " << atoi(cmd[1].c_str() + 1) << std::endl;
    }
}

void GuiApp::handlePic(const std::vector<std::string>& cmd)
{
    _map.addIncantationPos(atoi(cmd[1].c_str()), atoi(cmd[2].c_str()));
}

void GuiApp::handlePie(const std::vector<std::string>& cmd)
{
    _map.delIncantationPos(atoi(cmd[1].c_str()), atoi(cmd[2].c_str()));
}

void GuiApp::handlePfk(const std::vector<std::string>& cmd)
{
    std::cout << "Player " << cmd[1] << "layed an egg" << std::endl;
}

void GuiApp::handlePdr(const std::vector<std::string>& cmd)
{
    std::string itemName = "food";
    if (std::atoi(cmd[2].c_str()) > 0)
        itemName = _stoneNameList[std::atoi(cmd[2].c_str()) - 1];
    _map.dropResource(std::atoi(cmd[1].c_str() + 1), itemName);
}

void GuiApp::handlePgt(const std::vector<std::string>& cmd)
{
    std::string itemName = "food";
    if (std::atoi(cmd[2].c_str()) > 0)
        itemName = _stoneNameList[std::atoi(cmd[2].c_str()) - 1];
    _map.collectResource(std::atoi(cmd[1].c_str() + 1), itemName);
}

void GuiApp::handlePdi(const std::vector<std::string>& cmd)
{
    _map.delPlayer(atoi(cmd[1].c_str() + 1));
}

void GuiApp::handleEnw(const std::vector<std::string>& cmd)
{
    int eggId = std::atoi(cmd[1].c_str() + 1);
    int playerId = std::atoi(cmd[2].c_str() + 1);
    int x = std::atoi(cmd[3].c_str());
    int y = std::atoi(cmd[4].c_str());

    std::cout << "new egg with id = " << eggId << "\n";
    try {
        std::string teamName("");
        if (playerId != -1) {
            teamName = _map.getPlayerAtId(playerId).getTeamName();
        }
        _map.addEgg(x, y, eggId, teamName);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't handle command enw, no player found with ID: " << std::atoi(cmd[2].c_str() + 1) << std::endl;
    }
}

void GuiApp::handleEbo(const std::vector<std::string>& cmd)
{
    std::cout << "handle ebo\n";
    int eggId = std::atoi(cmd[1].c_str() + 1);
    std::cout << "egg:" << eggId << std::endl;

    try {
        std::string teamName = _map.getEggAtId(eggId).getTeamName();

        _map.delEgg(eggId);
        // _map.addPlayer(posX, posY, eggId, 0.0f, teamName, _teamColors[teamName]);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't handle command ebo, no egg found with ID: " << eggId << std::endl;
    }
}

void GuiApp::handleEdi(const std::vector<std::string>& cmd)
{
    std::cout << "handle edi\n";
    _map.delEgg(std::atoi(cmd[1].c_str() + 1));
}

void GuiApp::handleSgt(const std::vector<std::string>& cmd)
{
    _tickPerSecond = atoi(cmd[1].c_str());
    return;
}

void GuiApp::handleSst(const std::vector<std::string>& cmd)
{
    std::cout << "The tick per second has correctly been updated to: " << cmd[1] << std::endl;
    return;
}

void GuiApp::winingViewMode(int playerId)
{
    Player &player = _map.getPlayerAtId(playerId);
    Vector3 playerPos = player.getPosVec();

    _camera.target = playerPos;
    _camera.position = {playerPos.x, playerPos.y + 3, playerPos.z + 3};
    _camera.up = { 0.0f, 1.0f, 0.0f };
    _cam_mode = WINING_POV;
}

void GuiApp::handleSeg(const std::vector<std::string>& cmd)
{
    std::string teamName = cmd[1];

    std::cout << "GAME OVER\nThe team: " << teamName << " won" << std::endl;
    _drawWin = {true, teamName};

    const std::vector<Player> &vec = _map.getPlayerList();
    if (!vec.empty()) {
        for (auto &player : vec) {
            if (player.getTeamName() == teamName) {
                winingViewMode(player.getId());
                break;
            }
        }
    }
    return;
}

void GuiApp::handleSmg(const std::vector<std::string>& cmd)
{
    std::cout << "Server sent the message: " << cmd[1] << std::endl;
    return;
}

void GuiApp::handleSuc(const std::vector<std::string>& cmd)
{
    (void) cmd;
    std::cout << "Last command sent was unknown" << std::endl;
    return;
}

void GuiApp::handleSbp(const std::vector<std::string>& cmd)
{
    (void)cmd;
    std::cout << "Last command sent has bad parameters" << std::endl;
    return;
}

void GuiApp::initHandlers()
{
    _handlers = {
        { "msz", [this](const std::vector<std::string>& cmd){ handleMsz(cmd); } },
        { "bct", [this](const std::vector<std::string>& cmd){ handleBct(cmd); } },
        { "tna", [this](const std::vector<std::string>& cmd){ handleTna(cmd); } },
        { "pnw", [this](const std::vector<std::string>& cmd){ handlePnw(cmd); } },
        { "ppo", [this](const std::vector<std::string>& cmd){ handlePpo(cmd); } },
        { "plv", [this](const std::vector<std::string>& cmd){ handlePlv(cmd); } },
        { "pin", [this](const std::vector<std::string>& cmd){ handlePin(cmd); } },
        { "pex", [this](const std::vector<std::string>& cmd){ handlePex(cmd); } },
        { "pbc", [this](const std::vector<std::string>& cmd){ handlePbc(cmd); } },
        { "pic", [this](const std::vector<std::string>& cmd){ handlePic(cmd); } },
        { "pie", [this](const std::vector<std::string>& cmd){ handlePie(cmd); } },
        { "pfk", [this](const std::vector<std::string>& cmd){ handlePfk(cmd); } },
        { "pdr", [this](const std::vector<std::string>& cmd){ handlePdr(cmd); } },
        { "pgt", [this](const std::vector<std::string>& cmd){ handlePgt(cmd); } },
        { "pdi", [this](const std::vector<std::string>& cmd){ handlePdi(cmd); } },
        { "enw", [this](const std::vector<std::string>& cmd){ handleEnw(cmd); } },
        { "ebo", [this](const std::vector<std::string>& cmd){ handleEbo(cmd); } },
        { "edi", [this](const std::vector<std::string>& cmd){ handleEdi(cmd); } },
        { "sgt", [this](const std::vector<std::string>& cmd){ handleSgt(cmd); } },
        { "sst", [this](const std::vector<std::string>& cmd){ handleSst(cmd); } },
        { "seg", [this](const std::vector<std::string>& cmd){ handleSeg(cmd); } },
        { "smg", [this](const std::vector<std::string>& cmd){ handleSmg(cmd); } },
        { "suc", [this](const std::vector<std::string>& cmd){ handleSuc(cmd); } },
        { "sbp", [this](const std::vector<std::string>& cmd){ handleSbp(cmd); } }
    };
}
