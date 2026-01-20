/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Jun, 2025                                                     *
 * Title           - Raylib_zappy                                                       *
 * Description     -                                                                    *
 *     GUIApp                                                                           *
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

#include "GUIApp.hpp"

GuiApp::GuiApp(int windowWidth, int windowHeight, std::string host, int port, const char* title)
    : _windowWidth(windowWidth), _windowHeight(windowHeight), _hud(windowWidth, windowHeight), _protocol(host, port), _cam_mode(FREE), _block_view(false), _broadcastCircles()
{
    // test de la map en offline car j'ai pas le serv pour tester
    _mapWidth = 10;
    _mapHeight = 10;
    _map.initMapSize(_mapWidth, _mapHeight);
    // fin du test de la map en offline

    InitAudioDevice();
    _music = LoadMusicStream("assets/theme.ogg");
    if (_music.stream.buffer != NULL)
        PlayMusicStream(_music);

    _protocol.sendCommand("GRAPHIC\n");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(windowWidth, windowHeight, title);
    _background = LoadTexture("assets/DB_bg.jpg");
    _background.width = windowWidth;
    _background.height = windowHeight;
    
    _camera.position = {_mapWidth * 1.5f, _mapHeight * 0.5f, _mapHeight / 2.0f};
    _camera.target = {0.0f, 0.0f, _mapHeight / 2.0f};
    _camera.up = { 0.0f, 1.0f, 0.0f };
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;

    initHandlers();
    
    _map.initMapAssets();

    _hud.loadFont();

    SetMousePosition(windowWidth / 2, windowHeight / 2);
    DisableCursor();
    _selectedPlayer = nullptr;
    _selectedEgg = nullptr;
}

GuiApp::~GuiApp()
{
    if (_music.stream.buffer != NULL) {
        UnloadMusicStream(_music);
    }
    
    CloseAudioDevice();
    UnloadTexture(_background);
    _map.unloadMapModel();
    _hud.unloadFont();
    CloseWindow();
    _protocol.stop();
}

void GuiApp::changeViewMode()
{
    _cam_mode = (CAM_MODE)((_cam_mode + 1) % LAST_ELM);
    float maxMapSize = std::fmax(_mapWidth, _mapHeight);
    if (_cam_mode == TOP) {
        _camera.position = {_mapWidth / 2.0f, maxMapSize * 2, _mapHeight / 2.0f};
        _camera.target = {_mapWidth / 2.0f, 0.0, _mapHeight / 2.0f };
        _camera.up = { -1, 0, 0 };
        EnableCursor();
    } else if (_cam_mode == FREE) {
        _camera.position = {_mapWidth * 1.5f, _mapHeight * 0.5f, _mapHeight / 2.0f};
        _camera.target = {0.0f, 0.0f, _mapHeight / 2.0f};
        _camera.up = { 0.0f, 1.0f, 0.0f };
        DisableCursor();
    } else if (_cam_mode == PLAYER_POV) {
        EnableCursor();
        _followingPlayer = true;
    }
}

void GuiApp::handleEvents()
{
    if (_cam_mode == FREE && !_block_view) {
        UpdateCamera(&_camera, CAMERA_FREE);
    } else if (_cam_mode == WINING_POV) {
        UpdateCamera(&_camera, CAMERA_ORBITAL);
    }

    if (_followingPlayer && _cam_mode == PLAYER_POV) {
        updatePlayerPOV();
    }

    if (_protocol.pollResponse(_lastResponse)) {
        handleCommand(_lastResponse);
        _lastResponse = "caca";
    }

    if (IsKeyPressed(KEY_C) && _cam_mode != PLAYER_POV && _cam_mode != WINING_POV) {
        changeViewMode();
    }

    if (IsKeyPressed(KEY_U)) {
        std::vector<std::string> cmd = splitLine("pbc #12 hello\n");
        handlePbc(cmd);
    }

    if (_cam_mode == FREE && !_block_view && (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT))) {
        EnableCursor();
        _block_view = true;
    } else if (_cam_mode == FREE && _block_view && (IsKeyReleased(KEY_LEFT_SHIFT) || IsKeyReleased(KEY_RIGHT_SHIFT))) {
        DisableCursor();
        _block_view = false;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        
        if (_selectedPlayer && CheckCollisionPointRec(mousePos, _povButton)) {
            if (_followingPlayer && _followedPlayerId == _selectedPlayer->getId()) {
                // Stop following
                _followingPlayer = false;
                _followedPlayerId = -1;
                _cam_mode = FREE;
                changeViewMode();
            } else {
                // Start following
                _followingPlayer = true;
                _followedPlayerId = _selectedPlayer->getId();
                _cam_mode = PLAYER_POV;
                updatePlayerPOV();
            }
        } else if (_cam_mode != PLAYER_POV) {
            Ray ray = GetMouseRay(mousePos, _camera);
            _selectedPlayer = _map.rayCollidePlayer(ray);
            _selectedEgg = _map.rayCollideEgg(ray);
        }
    }
    
    if (!_selectedPlayer && _followingPlayer) {
        _followingPlayer = false;
        _followedPlayerId = -1;
        _cam_mode = FREE;
        changeViewMode();
    }
}

void GuiApp::handleCommand(const std::string &response)
{
    std::vector<std::string> command = splitLine(response);

    auto it = _handlers.find(command[0]);
    if (it != _handlers.end()) {
        return it->second(command);
    }
}

std::vector<std::string> GuiApp::splitLine(const std::string &line)
{
    std::vector<std::string> array;
    std::size_t start = 0;
    std::size_t end = line.find(" ");

    while (end != std::string::npos) {
        array.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(" ", start);
    }
    
    array.push_back(line.substr(start));
    return array;
}

void GuiApp::render()
{
    if (_music.stream.buffer != NULL)
        UpdateMusicStream(_music);
    BeginDrawing();
    ClearBackground(WHITE);
    DrawTexture(_background, 0, 0, _map.getBaseColor());

    BeginMode3D(_camera);
    _map.updateShenronAnims(GetFrameTime());
    _map.draw(_tickPerSecond);
    _map.drawAnthony(_camera);
    _broadcastCircles.drawCircles();
    EndMode3D();

    // Draw HUD here
    if (_drawWin.first) {
        std::string teamName = _drawWin.second;
        std::transform(teamName.begin(), teamName.end(), teamName.begin(),
                   [](unsigned char c) { return std::toupper(c); });

        _hud.drawWin(teamName, _teamColors[_drawWin.second]);
    } else {
        // Doesn't draw HUD when we won
        _hud.draw(_map);

        if (_selectedPlayer) {
            _hud.drawPlayerInfo(_selectedPlayer, _followingPlayer, _followedPlayerId, &_povButton);
        }
        if (_selectedEgg)
            _hud.drawEggInfo(_selectedEgg);
    }
    EndDrawing();
}

void GuiApp::run()
{
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        handleEvents();
        render();
    }
}

void GuiApp::AskServerForUpdate()
{
    std::string tmpPlayerId;

    // Boucle indexée correcte
    for (unsigned int i = 0; i < _map.getPlayerList().size(); i++) {
        tmpPlayerId = std::to_string(_map.getPlayerList()[i].getId());
        _protocol.sendCommand("pin " + tmpPlayerId + "\n");
    }
}

    void GuiApp::updatePlayerPOV()
    {
        if (!_followingPlayer || _followedPlayerId == -1)
            return;
        
        try {
            Player& player = _map.getPlayerAtId(_followedPlayerId);
            
            float heightOffset = 1.5f;
            float distanceBehind = 2.0f;
            
            float angleRad = player.getRotation() * DEG2RAD;
            
            Vector3 targetPosition = {
                player.getPosX() - sinf(angleRad) * distanceBehind,
                player.getPosY() + heightOffset,
                player.getPosZ() - cosf(angleRad) * distanceBehind
            };
            
            float lerpFactor = 0.1f;
            _camera.position.x = _camera.position.x + (targetPosition.x - _camera.position.x) * lerpFactor;
            _camera.position.y = _camera.position.y + (targetPosition.y - _camera.position.y) * lerpFactor;
            _camera.position.z = _camera.position.z + (targetPosition.z - _camera.position.z) * lerpFactor;
            
            float lookDistance = 5.0f;
            Vector3 targetLook = {
                player.getPosX() + sinf(angleRad) * lookDistance,
                player.getPosY() + 0.5f,
                player.getPosZ() + cosf(angleRad) * lookDistance
            };
            
            _camera.target.x = _camera.target.x + (targetLook.x - _camera.target.x) * lerpFactor;
            _camera.target.y = _camera.target.y + (targetLook.y - _camera.target.y) * lerpFactor;
            _camera.target.z = _camera.target.z + (targetLook.z - _camera.target.z) * lerpFactor;
            
            _camera.up = { 0.0f, 1.0f, 0.0f };
            
        } catch (const std::runtime_error& e) {
            _followingPlayer = false;
            _followedPlayerId = -1;
            _cam_mode = FREE;
            changeViewMode();
            EnableCursor();
        }
    }
