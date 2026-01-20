/*
** EPITECH PROJECT, 2025
** zappy_mirror
** File description:
** Hud.cpp
*/

#include "Hud.hpp"

Hud::Hud(int windowWidth, int windowHeight)
    : _windowWidth(windowWidth), _windowHeight(windowHeight) {}

void Hud::loadFont()
{
    _font = LoadFont("assets/fonts/fors.ttf");
}

void Hud::unloadFont()
{
    UnloadFont(_font);
}

void Hud::draw(Map &map)
{
    drawResouceBox(map);  
}

void Hud::drawResouceBox(Map &map)
{
    Vector2 pos = { _windowWidth * 0.01f, _windowHeight * 0.01f };

    std::vector<std::string> resources;
    resources.push_back("Players " + std::to_string(map.getTotalPlayer()));
    resources.push_back("Food " + std::to_string(map.getFood()));
    resources.push_back("Linemate " + std::to_string(map.getLinemate()));
    resources.push_back("Deraumere " + std::to_string(map.getDeraumere()));
    resources.push_back("Sibur " + std::to_string(map.getSibur()));
    resources.push_back("Mendiane " + std::to_string(map.getMendiane()));
    resources.push_back("Phiras " + std::to_string(map.getPhiras()));
    resources.push_back("Thystame " + std::to_string(map.getThystame()));

    drawTextBox(resources, pos, WHITE, "Map informations:");
}

void Hud::drawPlayerInfo(Player *player, bool followingPlayer, int followedPlayerId, Rectangle* povButton)
{
    std::vector<std::string> info;
    info.push_back("Level: " + std::to_string(player->getLevel()));
    info.push_back("Food: " + std::to_string(player->getResourceCount("food")));
    info.push_back("Linemate: " + std::to_string(player->getResourceCount("linemate")));
    info.push_back("Deraumere: " + std::to_string(player->getResourceCount("deraumere")));
    info.push_back("Sibur: " + std::to_string(player->getResourceCount("sibur")));
    info.push_back("Mendiane: " + std::to_string(player->getResourceCount("mendiane")));
    info.push_back("Phiras: " + std::to_string(player->getResourceCount("phiras")));
    info.push_back("Thystame: " + std::to_string(player->getResourceCount("thystame")));

    float posX = _windowWidth - 270.0f;

    drawTextBox(info, { posX, _windowHeight * 0.01f * 2 }, WHITE, "Player info           ");

    if (povButton) {
        float containerHeight = (1 + info.size()) * 36.450001f;
        float buttonY = (_windowHeight * 0.01f * 2) + containerHeight + 80.0f;
        float buttonWidth = 200.0f;
        float buttonHeight = 40.0f;
        float buttonX = posX + (_windowWidth * 0.13f - buttonWidth) / 2;
        
        *povButton = {buttonX, buttonY, buttonWidth, buttonHeight};
        
        // Draw the POV toggle button
        bool isFollowing = (followingPlayer && followedPlayerId == player->getId());
        drawButton(*povButton, isFollowing ? "Exit POV" : "Enter POV", isFollowing);
    }
}

/*
void Hud::drawEggInfo(Egg *egg)
{
    std::vector<std::string> info;
    info.push_back("Team: " + egg->getTeamName());

    drawTextBox(info, {_windowWidth - 100.0f, _windowHeight - 60.0f }, WHITE, "Egg info");
}*/

void Hud::drawEggInfo(Egg *egg)
{
    std::vector<std::string> info;
    info.push_back("Team: " + egg->getTeamName());

    float maxLineWidth = 0.0f;
    for (const auto& line : info) {
        float width = MeasureTextEx(_font, line.c_str(), 30, 2).x;
        if (width > maxLineWidth)
            maxLineWidth = width;
    }

    float boxWidth = maxLineWidth + 20.0f * 3;

    Vector2 pos = {
        _windowWidth - boxWidth - 20.0f,
        _windowHeight - 200.0f
    };

    drawTextBox(info, pos, WHITE, "Egg info:");
}

void Hud::drawButton(Rectangle bounds, const char* text, bool isToggled)
    {
        Color buttonColor = isToggled ? PURPLE : SKYBLUE;
        Color textColor = WHITE;
        Color borderColor = WHITE;
        
        Vector2 mousePos = GetMousePosition();
        bool isHovered = CheckCollisionPointRec(mousePos, bounds);
        
        if (isHovered) {
            buttonColor = isToggled ? Fade(PURPLE, 0.7f) : Fade(SKYBLUE, 0.7f);
        }
        
        DrawRectangleRounded(bounds, 0.1f, 15, Fade(buttonColor, 0.9f));
        DrawRectangleRoundedLines(bounds, 0.1f, 15, borderColor);
        
        Font font = getFont();
        Vector2 textSize = MeasureTextEx(font, text, 20, 2);
        Vector2 textPos = {
            bounds.x + (bounds.width - textSize.x) / 2,
            bounds.y + (bounds.height - textSize.y) / 2
        };
        DrawTextEx(font, text, textPos, 20, 2, textColor);
    }

void Hud::drawTextBox(const std::vector<std::string>& lines, Vector2 pos, Color textColor, const std::string& title)
{
    const float fontSize = 30.0f;
    const float lineSpacing = fontSize * 1.3f;
    const float padding = 20.0f;

    float maxLineWidth = 0.0f;
    for (const auto& line : lines) {
        float width = MeasureTextEx(_font, line.c_str(), fontSize, 2).x;
        if (width > maxLineWidth)
            maxLineWidth = width;
    }

    float titleHeight = 0.0f;
    float titleWidth = 0.0f;
    if (!title.empty()) {
        Vector2 titleSize = MeasureTextEx(_font, title.c_str(), fontSize, 2);
        titleHeight = lineSpacing;
        titleWidth = titleSize.x;
        if ((titleWidth > maxLineWidth + padding))
            maxLineWidth = titleWidth - padding;
    }

    float boxWidth = maxLineWidth + padding * 2 + (title.empty() ? 0.0f : padding);
    float boxHeight = titleHeight + lines.size() * lineSpacing + padding * 2;

    Rectangle box = { pos.x, pos.y, boxWidth, boxHeight };
    const float lineOffset = 7.0f;
    Rectangle line = {
        box.x + lineOffset,
        box.y + lineOffset,
        box.width - lineOffset * 2,
        box.height - lineOffset * 2
    };

    DrawRectangleRounded(box, 0.1f, 15, Fade(SKYBLUE, 0.9f));
    DrawRectangleRoundedLinesEx(line, 0.1f, 15, 2.0f, WHITE);

    Vector2 textPos = { pos.x + padding, pos.y + padding };

    if (!title.empty()) {
        DrawTextEx(_font, title.c_str(), textPos, fontSize, 2, textColor);
        textPos.y += lineSpacing;
        textPos.x += padding;
    }

    for (const auto& line : lines) {
        DrawTextEx(_font, line.c_str(), textPos, fontSize, 2, textColor);
        textPos.y += lineSpacing;
    }
}

void Hud::drawWin(std::string teamName, Color teamColor)
{
    float padding = 20;
    float spacing = 2.0f;
    float fontSize = 170.0f;
    const float lineOffset = 7.0f;
    std::string text = "TEAM " + teamName + " WON";

    Vector2 textSize = MeasureTextEx(_font, text.c_str(), fontSize, spacing);

    float boxWidth = textSize.x + 2 * padding;
    float boxHeight = textSize.y + 2 * padding;

    float boxX = (_windowWidth - boxWidth) / 2;
    float boxY = (_windowHeight - boxHeight) / 8;

    float textX = boxX + (boxWidth - textSize.x) / 2;
    float textY = boxY + (boxHeight - textSize.y) / 8;

    Rectangle box = {boxX, boxY, boxWidth, boxHeight};
    Rectangle line = {
        box.x + lineOffset,
        box.y + lineOffset,
        box.width - lineOffset * 2,
        box.height - lineOffset * 2
    };
    DrawRectangleRounded(box, 0.2f, 15, teamColor);
    DrawRectangleRoundedLinesEx(line, 0.1f, 15, 2.0f, WHITE);
    DrawTextEx(_font, text.c_str(), (Vector2){textX, textY}, fontSize, spacing, WHITE);
}

/*
void Hud::drawTextBlock(std::vector<std::string> text, Vector2 textPos, Color color)
{
    for (std::size_t i = 0; i < text.size(); i++) {
        DrawTextEx(_font, text[i].c_str(), textPos, 30, 2, color);
        textPos.y += 33;
    }
}

void Hud::drawLinedBox(Vector2 boxPos, float width, float height)
{
    float roundness = 0.1f;
    int segments = 15;
    float lineOffset = 7.0f;
    float lineThickness = 2.0f;

    Rectangle box = { boxPos.x, boxPos.y, width, height };
    Rectangle line = { box.x + lineOffset, box.y + lineOffset, box.width - lineOffset * 2, box.height - lineOffset * 2 };

    DrawRectangleRounded(box, roundness, segments, Fade(SKYBLUE, 0.9f));
    DrawRectangleRoundedLinesEx(line, roundness, segments, lineThickness, WHITE);
}

void Hud::drawContainer(std::string title, std::vector<std::string> content, Vector2 basePos)
{
    float paddingX = _windowWidth * 0.01f;
    float paddingY = _windowHeight * 0.01f;

    float textSize = ((_windowWidth * 0.0156f) + (_windowHeight * 0.0278f)) / 2.0f;
    float lineSpacing = _windowHeight * 0.0278f * 1.1f;

    float boxWidth = _windowWidth * 0.13f;
    float boxHeight = (1 + content.size()) * 36.450001;         // 36.450001 is a good ratio
    float spacing = 2;

    Vector2 titleSize = MeasureTextEx(_font, title.c_str(), textSize, spacing);
    Vector2 titlePos = {(boxWidth - titleSize.x) / 2 + basePos.x, basePos.y + spacing + paddingY};


    drawLinedBox(basePos, boxWidth, boxHeight);
    DrawTextEx(_font, title.c_str(), titlePos, textSize, spacing, WHITE);

    basePos.x += paddingX;
    basePos.y += lineSpacing + paddingY;

    drawTextBlock(content, basePos, WHITE);
}*/
