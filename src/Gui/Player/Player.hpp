/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Jun, 2025                                                     *
 * Title           - mirrorZappy                                                        *
 * Description     -                                                                    *
 *     Player                                                                           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_PLAYER_HPP
    #define INCLUDED_PLAYER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <raymath.h>

const int MAX_PARTICLES = 30;

struct modelInstance {
    Model model;
    Vector3 scale;
};

struct Particle {
    Vector3 position;
    Vector3 velocity;
    float lifetime;
    float size;
    Color color;
    bool active;
    float angle = 0.0f;
    float radius = 0.0f;
};

class Player
{
    private:
        modelInstance _currModel;
        Vector3 _pos;
        Vector3 _targetPos;
        float _moveSpeed = 5.0f;

        float _rotationAngle;
        Vector3 _rotationAxis = {0.0f, 1.0f, 0.0f};
        int _level = 1;
        int _id;
        std::string _teamName;

        std::map<std::string, int> _inventory;

        std::vector<Model> _transformationsModels;
        std::vector<Vector3> _transformationsScales;

        ModelAnimation* _anims = nullptr;
        int _animCount = 0;
        float _animFrame = 0;

        std::vector<Particle> _particles;
        Color _color;

    public:
        Player(float x, float y, float rotation, std::vector<Model> transformationsModels, std::vector<Vector3> transformationsScales, int id, const std::string &teamName, const Color& color);
        ~Player();
        Player(const Player&) = delete;
        Player(Player&&) noexcept;
        Player& operator=(Player&&) noexcept;
        Player& operator=(const Player&) = delete;

        void draw(Color baseColor);

        float getPosX() const { return _pos.x; };
        float getPosY() const { return _pos.y; };
        float getPosZ() const { return _pos.z; };
        Vector3 getPosVec() const { return {_pos.x, _pos.y, _pos.z}; };
        float getRotation() const { return _rotationAngle; };
        int getLevel() const { return _level; };
        int getId() const { return _id; };
        Model &getModel() { return _currModel.model; };
        std::string getTeamName() const { return _teamName; };
        
        void setPosX(int new_x) { _pos.x = new_x; };
        void setPosZ(int new_z) { _pos.z = new_z; };
        void setPosY(int new_y) { _pos.y = new_y; };
        void setTargetPos(float x, float y, float z) { _targetPos = {x, y, z}; }
        
        void setRotation(float new_rotation) { _rotationAngle = new_rotation; };
        void setModel(Model new_model) { _currModel.model = new_model; };
        void setTeamName(const std::string &teamName) { _teamName = teamName; };
        void setLevel(int level);
        void levelUp();

        void launchBroadcast(std::string message);
        void setInventory(int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame);
        void removeItemInventory(std::string itemIndex);
        void addItemInventory(std::string itemIndex);

        int getResourceCount(std::string type)
        {
            if (_inventory.find(type) == _inventory.end()) {
                return 0;
            }
            return _inventory[type];
        }

        void initParticles();
        void updateParticles(float);
        void drawParticles();
        void emitAuraParticles(Vector3 center, int amount);

        void update(float dt);
};
    
#endif
