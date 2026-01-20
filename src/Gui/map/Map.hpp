/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Jun, 2025                                                     *
 * Title           - Zappy                                                              *
 * Description     -                                                                    *
 *     Map                                                                              *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_MAP_HPP
    #define INCLUDED_MAP_HPP

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

#include "../Player/Player.hpp"
#include "../Egg/Egg.hpp"

class Map {
    public:
        Map();

        void initMapSize(float mapWidth, float mapHeight);

        void unloadMapModel();
        void initMapAssets();
        void initDBModels();
        void initTransfoModels();
        void initTransfoScales();
        void initHitbox();

        void addFood(int x, int y);
        void addStone(int x, int y, std::string stoneName = "anthony");
        void addPlayer(int x, int z, int playerId, float rotationAngle, const std::string &teamName, const Color& color);
        void addEgg(int x, int y, int eggId, const std::string &teamName);

        void delStone(int x, int y, std::string stoneName);
        void delFood(int x, int y);
        void delPlayer(int id);
        void delEgg(int eggId);
        void delEggAtPos(float x, float z);

        int countFood(int x, int y);
        int countStone(int x, int y, std::string stoneName);

        void draw(float tickPerSecond);
        void drawMap();
        void drawFood();
        void drawStone();
        void drawPlayer(float tickPerSecond);
        void drawEgg();
        void drawIncantation();
        void drawAnthony(Camera3D camera);

        void playerLevelUp(int id);

        void addIncantationPos(int x, int y);
        void delIncantationPos(int x, int y);

        Vector3 getLastIncantation() { return _incantationPos[_incantationPos.size() - 1]; };

        const std::vector<Player>& getPlayerList() const { return _playerList; }

        void setPlayerLevel(int level, int id);

        Player& getPlayerAtId(int id);
        Egg &getEggAtId(int id);

        Color getBaseColor() { return _baseColor; };

        void setPlayerPos(int playerId, int x, int y, float orientation);
        void setPlayerInventory(int playerId, int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame);

        void dropResource(int playerId, const std::string &itemName);
        void collectResource(int playerId, const std::string &itemName);

        Player *rayCollidePlayer(Ray &ray);
        Egg *rayCollideEgg(Ray &ray);

        int getFood() { return _nbFood; };
        int getLinemate() { return _nbLinemate; };
        int getDeraumere() { return _nbDeraumere; };
        int getSibur() { return _nbSibur; };
        int getMendiane() { return _nbMendiane; };
        int getPhiras() { return _nbPhiras; };
        int getThystame() { return _nbThystame; };
        size_t getTotalPlayer() { return _playerList.size(); };

        void triggerShenron(Vector3 pos);

        void updateShenronAnims(float dt);


    private:
        float _width;
        float _height;
        Texture2D _tex;
        Model _CastleModel;
        Model _SenzuModel;
        Model _DB1Model;
        Model _DB2Model;
        Model _DB3Model;
        Model _DB4Model;
        Model _DB5Model;
        Model _DB6Model;
        Model _DB7Model;

        Model _ShenronModel;
        std::vector<Vector3> _incantationPos;

        Sound _teleportSound;

        Color _baseColor = WHITE;

        std::vector<Model> _transformationsModels;
        std::vector<Vector3> _transformationsScales;
        
        struct _FoodInstance {
            float x, z;
            float rotationAngle;
            Vector3 rotationAxis;
        };

        struct _StoneInstance {
            float x, z;
            std::string name;
            Model *model;
        };

        int _nbFood = 0;
        int _nbLinemate = 0;
        int _nbDeraumere = 0;
        int _nbSibur = 0;
        int _nbMendiane = 0;
        int _nbPhiras = 0;
        int _nbThystame = 0;

        std::map<std::pair<int, int>, std::vector<_FoodInstance>> foodList;
        std::map<std::pair<int, int>, std::vector<_StoneInstance>> stoneList;

        std::vector<Player> _playerList;
        std::vector<Egg> _eggList;

        struct ShenronAnim {
            Vector3 pos;
            float timer;
            float duration;
            float squash;
            bool active;
        };
        std::vector<ShenronAnim> _shenronAnims;
    };

#endif
