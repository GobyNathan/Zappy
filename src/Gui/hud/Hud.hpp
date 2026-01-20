/*
** EPITECH PROJECT, 2025
** zappy_mirror
** File description:
** Hud.hpp
*/

#ifndef HUD_HPP
#define HUD_HPP

#include "../map/Map.hpp"
#include <raylib.h>
#include <string>
#include <vector>

class Hud {
    public:
        Hud(int windowWidth, int windowHeight);

        void loadFont();
        void unloadFont();

        void draw(Map &map);

        void drawResouceBox(Map &map);
        void drawPlayerInfo(Player *player, bool followingPlayer, int followedPlayerId, Rectangle* povButton);
        void drawEggInfo(Egg *egg);

        void drawButton(Rectangle bounds, const char* text, bool isToggled);

        void drawWin(std::string teamName, Color teamColor);

        void drawTextBox(const std::vector<std::string>& lines, Vector2 pos, Color textColor, const std::string &title);
        //void drawTextBlock(std::vector<std::string> text, Vector2 basePos, Color color);
        //void drawLinedBox(Vector2 boxPos, float width, float height);

        //void drawContainer(std::string title, std::vector<std::string> content, Vector2 basePos);

        Font getFont() const { return _font; }
    private:
        int _windowWidth;
        int _windowHeight;

        Font _font;
};

#endif
