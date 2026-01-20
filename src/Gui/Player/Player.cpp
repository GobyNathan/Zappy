/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Jun, 2025                                                     *
 * Title           - mirrorZappy                                                        *
 * Description     -                                                                    *
 *     Player                                                                           *
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

#include "Player.hpp"

Player::Player(float x, float y, float rotation, std::vector<Model> transformationsModels,
    std::vector<Vector3> transformationsScales, int id, const std::string &teamName, const Color &color)
{
    _transformationsModels = transformationsModels;
    _transformationsScales = transformationsScales;
    _currModel = {transformationsModels[0], transformationsScales[0]};

    _pos = (Vector3){x, 1.2f, y};
    _targetPos = _pos;    _rotationAngle = rotation;

    _id = id;
    _teamName = teamName;


    // Charge l'animation du modèle flying_goku.glb
    _anims = LoadModelAnimations("assets/transformations/flying_goku.glb", &_animCount);
    _animFrame = 0;

    initParticles();
    _color = color;
}

Player::~Player()
{
    if (_anims)
        UnloadModelAnimations(_anims, _animCount);
}

Player::Player(Player&& other) noexcept
    : _currModel(std::move(other._currModel)),
      _pos(other._pos),
      _rotationAngle(other._rotationAngle),
      _rotationAxis(other._rotationAxis),
      _level(other._level),
      _id(other._id),
      _teamName(std::move(other._teamName)),
      _inventory(std::move(other._inventory)),
      _transformationsModels(std::move(other._transformationsModels)),
      _transformationsScales(std::move(other._transformationsScales)),
      _anims(other._anims),
      _animCount(other._animCount),
      _animFrame(other._animFrame)
{
    other._anims = nullptr;
    other._animCount = 0;
    other._animFrame = 0;
    initParticles();
    _color = other._color;
}

Player& Player::operator=(Player&& other) noexcept
{
    if (this != &other) {
        if (_anims)
            UnloadModelAnimations(_anims, _animCount);
        _currModel = std::move(other._currModel);
        _pos = other._pos;
        _rotationAngle = other._rotationAngle;
        _rotationAxis = other._rotationAxis;
        _level = other._level;
        _id = other._id;
        _teamName = std::move(other._teamName);
        _inventory = std::move(other._inventory);
        _transformationsModels = std::move(other._transformationsModels);
        _transformationsScales = std::move(other._transformationsScales);
        _anims = other._anims;
        _animCount = other._animCount;
        _animFrame = other._animFrame;
        other._anims = nullptr;
        other._animCount = 0;
        other._animFrame = 0;
    }
    return *this;
}

void Player::draw(Color baseColor)
{
    // if (_anims && _animCount > 0) {
    //     _animFrame += 1.0f; // ou deltaTime * speed pour ajuster la vitesse
    //     if (_animFrame >= _anims[0].frameCount)
    //         _animFrame = 0;
    //     UpdateModelAnimation(_currModel.model, _anims[0], (int)_animFrame);
    // }
    DrawModelEx(_currModel.model, _pos, _rotationAxis, _rotationAngle, _currModel.scale, baseColor);
    drawParticles();
}

void Player::levelUp()
{
    _level++;
    _currModel = {_transformationsModels[_level-1], _transformationsScales[_level-1]};
}

void Player::setLevel(int level)
{
    _level = level;
    _currModel = {_transformationsModels[_level-1], _transformationsScales[_level-1]};
}

void Player::launchBroadcast(std::string message)
{
    std::cout << "message broadcasted: " << message << "\n";
}

void Player::setInventory(int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame)
{
    _inventory["food"] = food;
    _inventory["linemate"] = linemate;
    _inventory["deraumere"] = deraumere;
    _inventory["sibur"] = sibur;
    _inventory["mendiane"] = mendiane;
    _inventory["phiras"] = phiras;
    _inventory["thystame"] = thystame;
}

void Player::removeItemInventory(std::string itemName)
{
    if (_inventory[itemName] > 0)
        _inventory[itemName]--;
}

void Player::addItemInventory(std::string itemIndex)
{
    _inventory[itemIndex]++;
}

void Player::initParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Particle particule = {};
        particule.active = false;
        _particles.push_back(particule);
    }
}

void Player::updateParticles(float dt)
{
    for (size_t i = 0; i < _particles.size(); i++) {
        if (_particles[i].active) {
            // Spiral movement
            _particles[i].angle += 0.3f * dt; // Speed of rotation

            // Center is the player's position (optionally offset in Y)
            Vector3 center = _pos;
            center.y += 0.5f;

            _particles[i].position.x = center.x + cosf(_particles[i].angle) * _particles[i].radius;
            _particles[i].position.z = center.z + sinf(_particles[i].angle) * _particles[i].radius;
        }
    }
}

void Player::emitAuraParticles(Vector3 center, int amount)
{
    for (size_t i = 0; i < _particles.size() && amount > 0; i++) {
        if (!_particles[i].active) {
            _particles[i].active = true;

            float angle = GetRandomValue(0, 360) * DEG2RAD;
            float radius = GetRandomValue(5, 50) / 100.0f; // 0.1 to 0.3

            _particles[i].angle = angle;
            _particles[i].radius = radius;
 
            float x = center.x + cosf(angle) * radius;
            float z = center.z + sinf(angle) * radius;

            float min = -0.1f, max = 0.2f;
            float randomFloat = min + (GetRandomValue(0, 1000000) / 1000000.0f) * (max - min);
            float y = center.y + randomFloat;
 
            _particles[i].position = { x, y, z };
            _particles[i].velocity = {
                GetRandomValue(-5, 5) / 300.0f,
                GetRandomValue(10, 20) / 300.0f,
                GetRandomValue(-5, 5) / 300.0f
            };
            _particles[i].size = 0.015f;
            _particles[i].color = _color;
            amount--;
        }
    }
}

void Player::drawParticles()
{
    float dt = GetFrameTime();
    Vector3 pos = _pos;
    pos.y += 0.2;

    emitAuraParticles(pos, 2);
    updateParticles(dt);

    for (size_t i = 0; i < _particles.size(); i++) {
        if (_particles[i].active) {
            DrawSphere(_particles[i].position, _particles[i].size, _particles[i].color);
        }
    }
}

void Player::update(float dt)
{
    float dist = Vector3Distance(_pos, _targetPos);
    if (dist > 0.01f) {
        Vector3 dir = Vector3Normalize(Vector3Subtract(_targetPos, _pos));
        float step = _moveSpeed * (dt * 0.001);
        if (step > dist) step = dist;
        _pos = Vector3Add(_pos, Vector3Scale(dir, step));
    } else {
        _pos = _targetPos;
    }
    updateParticles(dt);
}
