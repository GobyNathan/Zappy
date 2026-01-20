/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Jun, 2025                                                     *
 * Title           - mirrorZappy                                                        *
 * Description     -                                                                    *
 *     Map                                                                              *
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

#include "Map.hpp"

Map::Map() {}

void Map::initMapSize(float mapWidth, float mapHeight)
{
    _width = mapWidth;
    _height = mapHeight;

    _teleportSound = LoadSound("assets/teleport.mp3");
}


void Map::unloadMapModel()
{
    UnloadModel(_CastleModel);
}

void Map::initMapAssets()
{
    _CastleModel = LoadModel("assets/towerOfGod.glb");
    _SenzuModel = LoadModel("assets/senzu_bean.glb");

    initDBModels();

    initTransfoModels();
    initTransfoScales();

    _ShenronModel = LoadModel("assets/shenron.glb");

    _tex = LoadTexture("assets/GodTowerFloor.png");
}

void Map::initDBModels()
{
    _DB1Model = LoadModel("assets/Dragon Balls/DB1.glb");
    _DB2Model = LoadModel("assets/Dragon Balls/DB2.glb");
    _DB3Model = LoadModel("assets/Dragon Balls/DB3.glb");
    _DB4Model = LoadModel("assets/Dragon Balls/DB4.glb");
    _DB5Model = LoadModel("assets/Dragon Balls/DB5.glb");
    _DB6Model = LoadModel("assets/Dragon Balls/DB6.glb");
    _DB7Model = LoadModel("assets/Dragon Balls/DB7.glb");
}

void Map::initTransfoModels()
{
    for (int i = 0; i < 8; i++) {
        _transformationsModels.push_back((Model){});
    }
    _transformationsModels[0] = LoadModel("assets/transformations/goku.glb");
    _transformationsModels[1] = LoadModel("assets/transformations/goku_ssj.glb");
    _transformationsModels[2] = LoadModel("assets/transformations/goky_ssj2.glb");
    _transformationsModels[3] = LoadModel("assets/transformations/goku_ssj3.glb");
    _transformationsModels[4] = LoadModel("assets/transformations/goku_ssj4.glb");
    _transformationsModels[5] = LoadModel("assets/transformations/goku_god.glb");
    _transformationsModels[6] = LoadModel("assets/transformations/goku_ssj_blue.glb");
    _transformationsModels[7] = LoadModel("assets/transformations/goku_ssj5.glb");
}

void Map::initTransfoScales()
{
    for (int i = 0; i < 8; i++) {
        _transformationsScales.push_back((Vector3){});
    }
    _transformationsScales[0] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[1] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[2] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[3] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[4] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[5] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[6] = {0.2f, 0.2f, 0.2f};
    _transformationsScales[7] = {0.2f, 0.2f, 0.2f};
}

void Map::draw(float tickPerSecond)
{
    _baseColor = (_incantationPos.size() == 0) ? WHITE : (Color){65, 67, 90, 255};

    drawMap();

    drawFood();
    drawStone();

    drawPlayer(tickPerSecond);
    drawEgg();

    drawIncantation();

    DrawModel(_CastleModel, (Vector3){(float)(0.1 * _width), 0.5f, _height/2 - 0.5f}, 10.0f * std::max(_height, _width), _baseColor);
}

void Map::drawMap()
{

    static Mesh mesh = GenMeshCube(1, 0.5f, 1);
    static Model cubeModel = LoadModelFromMesh(mesh);
    static bool materialSet = false;

    if (!materialSet) {
        cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = _tex;
        materialSet = true;
    }

    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            DrawModel(cubeModel, (Vector3){ (float)i, 0.5f, (float)j }, 1.0f, _baseColor); // c'est pas aligné quand la width est paire
            DrawCubeWires({(float)i, 0.5f, (float)j}, 1, 0.5f, 1, BLACK); // c'est pas aligné tout court enfaite
        }
    }
}

void Map::drawFood()
{
    for (const auto& cell : foodList) {
        for (const auto& food : cell.second) {
            Vector3 position = {food.x, 0.9f, food.z};
            DrawModelEx(_SenzuModel, position, food.rotationAxis, food.rotationAngle, (Vector3){0.02f, 0.02f, 0.02f}, _baseColor);
        }
    }
}

void Map::drawStone()
{
    for (const auto& cell : stoneList) {
        for (const auto& stone : cell.second) {
            DrawModel(*stone.model, (Vector3){stone.x, 0.9f, stone.z}, 0.1f, _baseColor);
        }
    }
}

void Map::drawPlayer(float tickPerSecond)
{
    for (unsigned int i = 0; i < _playerList.size(); i++) {
        _playerList[i].update(tickPerSecond);
        _playerList[i].draw(_baseColor);
    }
}

void Map::drawEgg()
{
    for (std::size_t i = 0; i < _eggList.size(); i++)
        _eggList[i].draw(_baseColor);
}

void Map::drawIncantation()
{
    float baseY = -5.0f; // point de départ sous la map
    for (unsigned int i = 0; i < _incantationPos.size(); i++) {
        ShenronAnim* anim = nullptr;
        for (auto& a : _shenronAnims) {
            if (Vector3Distance(a.pos, _incantationPos[i]) < 0.1f) {
                anim = &a;
                break;
            }
        }
        Vector3 pos = _incantationPos[i];
        float squash = 1.0f;
        float alpha = 1.0f;

        if (anim && anim->active) {
            // Apparition animée
            float t = anim->timer / anim->duration;
            float y = baseY * (1.0f - t) + pos.y * t;
            // Oscillation pendant l'animation
            float floatAmplitude = 3.0f;
            float floatSpeed = 2.0f;
            y += sinf(anim->timer * floatSpeed) * floatAmplitude;
            pos.y = y;
            squash = 1.0f + 1.5f * (1.0f - t) * sinf(t * 3.14f);
            alpha = t;
        } else {
            // Flottement permanent après l'animation
            float floatAmplitude = 0.01f;
            float floatSpeed = 3.f;
            pos.y += sinf(GetTime() * floatSpeed) * floatAmplitude;
            squash = 1.0f;
            alpha = 1.0f;
        }
        Color color = Fade(WHITE, alpha);
        DrawModelEx(_ShenronModel, pos, {0.0f, 1.0f, 0.0f}, 50.0f, {3.0f, 3.0f * squash, 3.0f}, color);
    }
}

void Map::drawAnthony(Camera3D camera)
{
    static Texture2D anthonyTex = LoadTexture("assets/anthony_no_background.png");
    Vector3 anthonyPos = {std::max(_height, _width)* -0.45f, 0.47f * std::max(_height, _width), (float)(_height) / 2.0f - 0.5f};
    DrawBillboard(camera, anthonyTex, anthonyPos, std::max(_height, _width) / 10, _baseColor);
}

void Map::addFood(int x, int z)
{
    float offsetX = ((float)GetRandomValue(-400, 400)) / 1000.0f;
    float offsetZ = ((float)GetRandomValue(-400, 400)) / 1000.0f;
    float posX = x + offsetX;
    float posZ = z + offsetZ;
    float rotationAngle = (float)GetRandomValue(0, 360);
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};

    _FoodInstance food;
    food.x = posX;
    food.z = posZ;
    food.rotationAngle = rotationAngle;
    food.rotationAxis = rotationAxis;

    foodList[{x, z}].push_back(food);
    _nbFood++;
}

void Map::addStone(int x, int z, std::string stoneName)
{
    float offsetX = ((float)GetRandomValue(-400, 400)) / 1000.0f;
    float offsetZ = ((float)GetRandomValue(-400, 400)) / 1000.0f;
    float posX = x + offsetX;
    float posZ = z + offsetZ;

    _StoneInstance stone;
    stone.x = posX;
    stone.z = posZ;
    stone.name = stoneName;

    if (stoneName == "linemate") {
        stone.model = &_DB1Model;
        _nbLinemate++;
    } else if (stoneName == "deraumere") {
        stone.model = &_DB2Model;
        _nbDeraumere++;
    } else if (stoneName == "sibur") {
        stone.model = &_DB3Model;
        _nbSibur++;
    } else if (stoneName == "mendiane") {
        stone.model = &_DB4Model;
        _nbMendiane++;
    } else if (stoneName == "phiras") {
        stone.model = &_DB5Model;
        _nbPhiras++;
    } else if (stoneName == "thystame") {
        stone.model = &_DB6Model;
        _nbThystame++;
    } else {
        std::cerr << "Bad stone name\n";
        return;
    }
    stoneList[{x, z}].push_back(stone);
}

void Map::addPlayer(int x, int z, int playerId, float rotationAngle, const std::string &teamName, const Color& color)
{
    std::cout << "new player with id : " << playerId << " pnw\n";
    Player newPlayer(x, z, rotationAngle, _transformationsModels, _transformationsScales, playerId, teamName, color);
    PlaySound(_teleportSound);

    // _playerList.push_back(newPlayer);
    _playerList.emplace_back(x, z, rotationAngle, _transformationsModels, _transformationsScales, playerId, teamName, color);
}

void Map::addEgg(int x, int y, int eggId, const std::string &teamName)
{
    std::cout << "addEgg: id=" << eggId << " pos=" << x << "," << y << std::endl;
    Egg newEgg(x, y, eggId, teamName);
    _eggList.push_back(newEgg);
}

void Map::delFood(int x, int z)
{
    auto it = foodList.find({x, z});
    if (it != foodList.end() && !it->second.empty()) {
        it->second.pop_back();
        if (it->second.empty()) {
            foodList.erase(it);
        }
    }

    if (_nbFood > 0)
        _nbFood--;
}

void Map::delStone(int x, int z, std::string stoneName)
{
    auto it = stoneList.find({x, z});
    if (it != stoneList.end()) {
        auto& vec = it->second;
        auto found = std::find_if(vec.begin(), vec.end(), [&](const _StoneInstance& s){ return s.name == stoneName; });
        if (found != vec.end()) {
            vec.erase(found);
            if (vec.empty())
                stoneList.erase(it);
        }
    } else {
        std::cerr << "Stone not found on this tile\n";
    }

    if (stoneName == "linemate")
        _nbLinemate--;
    if (stoneName == "deraumere")
        _nbDeraumere--;
    if (stoneName == "sibur")
        _nbSibur--;
    if (stoneName == "mendiane")
        _nbMendiane--;
    if (stoneName == "phiras")
        _nbPhiras--;
    if (stoneName == "thystame")
        _nbThystame--;
}

void Map::delPlayer(int id)
{
    for (auto it = _playerList.begin(); it != _playerList.end(); ++it) {
        if (it->getId() == id) {
            _playerList.erase(it);
            return;
        }
    }
}

void Map::delEgg(int eggId)
{
    for (auto it = _eggList.begin(); it != _eggList.end(); it++) {
        if (it->getId() == eggId) {
            _eggList.erase(it);
            return;
        }
    }
    std::cerr << "Egg not found for deletion: " << eggId << std::endl;
}

void Map::delEggAtPos(float x, float z)
{
    for (auto it = _eggList.begin(); it != _eggList.end(); it++) {
        if (it->getPosVec() == (Vector3){x, 0.975f, z}) {
            _eggList.erase(it);
            return;
        }
    }
    std::cerr << "Egg not found for deletion at: " << x << " " << z << std::endl;
}

void Map::playerLevelUp(int id)
{
    for (unsigned int i = 0; i < _playerList.size(); i++) {
        if (_playerList[i].getId() == id) {
            _playerList[i].levelUp();
            return;
        }
    }
    std::cerr << "No player found at ID (" << id << ")" << std::endl;
}

int Map::countFood(int x, int z)
{
    auto it = foodList.find({x, z});
    if (it != foodList.end())
        return it->second.size();
    return 0;
}

int Map::countStone(int x, int z, std::string stoneName)
{
    auto it = stoneList.find({x, z});
    if (it != stoneList.end()) {
        return std::count_if(it->second.begin(), it->second.end(),
            [&](const _StoneInstance& s){ return s.name == stoneName; });
    }
    return 0;
}

void Map::setPlayerLevel(int level, int id)
{
    try {
        getPlayerAtId(id).setLevel(level);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't set player level, no player found with ID: " << id << std::endl;
    }
}

Player& Map::getPlayerAtId(int id)
{
    for (std::size_t i = 0; i < _playerList.size(); i++) {
        if (_playerList[i].getId() == id) {
            return _playerList[i];
        }
    }
    throw std::runtime_error("No player found with ID: " + std::to_string(id));
}

Egg &Map::getEggAtId(int id)
{
    for (std::size_t i = 0; i < _eggList.size(); i++) {
        std::cout << "getEggAtId: id: " << _eggList[i].getId() << "\n";
        if (_eggList[i].getId() == id) {
            std::cout << "egg returned : " << _eggList[i].getId() << "\n";
            return _eggList[i];
        }
    }
    std::cerr << "No egg found with ID: " << id << "\n";
    throw std::runtime_error("No egg found with ID: " + std::to_string(id));
}

void Map::setPlayerPos(int playerId, int x, int y, float orientation)
{
    std::map<int, int> angles = {
        {1, 180},
        {2, 90},
        {3, 0},
        {4, -90},
    };
    try {
        Player& p = getPlayerAtId(playerId);
        p.setTargetPos(x, 1.2f, y);
        p.setRotation(angles[orientation]);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't set player position, no player found with ID: " << playerId << std::endl;
    }
}

void Map::setPlayerInventory(int playerId, int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame)
{
    try {
        getPlayerAtId(playerId).setInventory(food, linemate, deraumere, sibur, mendiane, phiras, thystame);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't set player inventory, no player found with ID: " << playerId << std::endl;
    }
}

void Map::dropResource(int playerId, const std::string &itemName)
{
    try {
        getPlayerAtId(playerId).removeItemInventory(itemName);

        int posX = getPlayerAtId(playerId).getPosX();
        int posY = getPlayerAtId(playerId).getPosZ();
        if (itemName == "food")
            addFood(posX, posY);
        else
            addStone(posX, posY, itemName);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't drop resource, no player found with ID: " << playerId << std::endl;
    }
}

void Map::collectResource(int playerId, const std::string &itemName)
{
    try {
        getPlayerAtId(playerId).addItemInventory(itemName);

        int posX = getPlayerAtId(playerId).getPosX();
        int posY = getPlayerAtId(playerId).getPosZ();
        if (itemName == "food")
            delFood(posX, posY);
        else
            delStone(posX, posY, itemName);
    } catch (const std::runtime_error &error) {
        std::cerr << "Error: Couldn't collect resource, no player found with ID: " << playerId << std::endl;
    }
}

void Map::addIncantationPos(int x, int z)
{
    _incantationPos.push_back((Vector3){ (float)x, 0.5, (float)z});
    triggerShenron({(float)x, 0.5, (float)z});
}

void Map::delIncantationPos(int x, int z)
{
    for (auto it = _incantationPos.begin(); it != _incantationPos.end(); ++it)
    {
        if (it->x == x && it->z == z) {
            _incantationPos.erase(it);
            return;
        }
    }
}

Player *Map::rayCollidePlayer(Ray &ray)
{
    for (size_t i = 0; i < _playerList.size(); i++) {
        Vector3 pos = _playerList[i].getPosVec();

        BoundingBox box = {
            {pos.x - 0.2f, pos.y, pos.z - 0.2f},
            {pos.x + 0.2f, pos.y + 0.8f, pos.z + 0.2f}
        };

        RayCollision rayCol = GetRayCollisionBox(ray, box);

        if (rayCol.hit) {
            return &_playerList[i];
        }
    }
    return nullptr;
}

Egg *Map::rayCollideEgg(Ray &ray)
{
    for (std::size_t i = 0; i < _eggList.size(); i++) {
        Vector3 pos = _eggList[i].getPosVec();

        BoundingBox box = {
            { pos.x - 0.2f, pos.y, pos.z - 0.2f },
            { pos.x + 0.2f, pos.y + 0.8f, pos.z + 0.2f }
        };

        RayCollision rayCol = GetRayCollisionBox(ray, box);

        if (rayCol.hit)
            return &_eggList[i];
    }
    return nullptr;
}

void Map::triggerShenron(Vector3 pos) {
    _shenronAnims.push_back({pos, 0.0f, 0.1f, 1.5f, true});
}

void Map::updateShenronAnims(float dt) {
    for (auto& anim : _shenronAnims) {
        if (!anim.active) continue;
        anim.timer += dt;
        if (anim.timer > anim.duration) anim.active = false;
    }
}
