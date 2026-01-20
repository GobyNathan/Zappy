/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Jun, 2025                                                     *
 * Title           - zappy_mirror                                                       *
 * Description     -                                                                    *
 *     BroadcastCircle                                                                  *
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

#ifndef INCLUDED_BROADCASTCIRCLE_HPP
    #define INCLUDED_BROADCASTCIRCLE_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <raylib.h>
#include <chrono>
#include <math.h>
#include "rlgl.h"

class Circle
{
    private:
        Vector3 _center;
        float _growthSpeed;
        float _radius;
    public:
        Circle(Vector3 center) : _center(center), _growthSpeed(3), _radius(0) {};
        Vector3 &getCenter() { return _center; };
        float &getRadius() { return _radius; };
        void increaseRadius(float inc) { _radius += inc; };
        float getGrowthSpeed() { return _growthSpeed; };
};

class BroadcastCircles
{
    private:
        std::vector<std::unique_ptr<Circle>> _circles;
        float _maxRadius;
        std::chrono::steady_clock::time_point _last_time;
    protected:
    public:
        BroadcastCircles()
        {
            _maxRadius = 10;
            _last_time = std::chrono::steady_clock::now();
        };
        ~BroadcastCircles() = default;

        void setMaxRadius(float rad) { _maxRadius = rad; };
        void addCircle(std::unique_ptr<Circle> circ) { _circles.push_back(std::move(circ)); };

        void DrawCylinderWireCustom(Vector3 position, float radius, float height, int slices, Color color)
        {
            float angleStep = 360.0f / slices;
            float halfHeight = height / 2.0f;

            Vector3 top[slices];
            Vector3 bottom[slices];

            for (int i = 0; i < slices; i++) {
                float angle = DEG2RAD * (i * angleStep);
                float x = sinf(angle) * radius;
                float z = cosf(angle) * radius;

                top[i] = (Vector3){ position.x + x, position.y + halfHeight, position.z + z };
                bottom[i] = (Vector3){ position.x + x, position.y - halfHeight, position.z + z };
            }

            rlPushMatrix();
            rlSetLineWidth(2.0f);
            rlBegin(RL_LINES);
            rlColor4ub(color.r, color.g, color.b, color.a);

            for (int i = 0; i < slices; i++) {
                int next = (i + 1) % slices;

                rlVertex3f(top[i].x, top[i].y, top[i].z);
                rlVertex3f(top[next].x, top[next].y, top[next].z);

                rlVertex3f(bottom[i].x, bottom[i].y, bottom[i].z);
                rlVertex3f(bottom[next].x, bottom[next].y, bottom[next].z);

                rlVertex3f(top[i].x, top[i].y, top[i].z);
                rlVertex3f(bottom[i].x, bottom[i].y, bottom[i].z);
            }

            rlEnd();
            rlSetLineWidth(1.0f);
            rlPopMatrix();
        }

        void drawCircles()
        {
            auto now = std::chrono::steady_clock::now();
            float delta = std::chrono::duration<float>(now - _last_time).count();
            _last_time = now;
 
            for (auto it = _circles.begin(); it != _circles.end();) {
                auto &circle = *it;

                if (circle->getRadius() > _maxRadius) {
                    it = _circles.erase(it);
                } else {
                    DrawCylinderWireCustom(circle->getCenter(), circle->getRadius(), 0.01f, 64, PURPLE);
                    circle->increaseRadius(delta * circle->getGrowthSpeed());
                    it++;
                }
            }
        }
};




#endif

