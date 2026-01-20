/*
** EPITECH PROJECT, 2025
** zappy_mirror
** File description:
** Egg.cpp
*/

#include "Egg.hpp"

Egg::Egg(float x, float y, int id, const std::string &teamName)
{
    _pos = (Vector3){ x, 1.05f, y };
    _id = id;
    _teamName = teamName;
    _model = { LoadModel("assets/golden_egg.glb"), (Vector3){ 1.0f, 1.0f, 1.0f } };
    _scale = 0.3f;
}

void Egg::draw(Color baseColor)
{
    DrawModel(_model.model, _pos, _scale, baseColor);
}
