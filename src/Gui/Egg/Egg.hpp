/*
** EPITECH PROJECT, 2025
** zappy_mirror
** File description:
** Egg.hpp
*/

#ifndef EGG_HPP
#define EGG_HPP

#include "../Player/Player.hpp"

class Egg {
    private:
        modelInstance _model;
        float _scale;
        Vector3 _pos;
        int _id;
        std::string _teamName;

    public:
        Egg(float x, float y, int id, const std::string &teamName);
        ~Egg() = default;

        void draw(Color baseColor);

        float getPosX() const { return _pos.x; };
        float getPosY() const { return _pos.y; };
        float getPosZ() const { return _pos.z; };
        Vector3 getPosVec() const { return _pos; };
        int getId() const { return _id; };
        std::string getTeamName() const { return _teamName; };
};

#endif
