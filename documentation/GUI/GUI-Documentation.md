# Zappy GUI Documentation

## Table of Contents
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Installation and Setup](#installation-and-setup)
4. [Usage](#usage)
5. [Features](#features)
6. [Protocol Communication](#protocol-communication)
7. [API Reference](#api-reference)
8. [File Structure](#file-structure)
9. [Controls](#controls)
10. [Troubleshooting](#troubleshooting)

## Overview

The Zappy GUI is a 3D graphical client for the Zappy network game, built with C++ and Raylib. It provides real-time visualization of the game world, players, resources, and game events with a Dragon Ball Z themed interface.

### Key Features
- **3D Visualization**: Immersive 3D world rendering
- **Real-time Updates**: Live synchronization with the game server
- **Multiple Camera Modes**: Free camera, top-down view, and player POV
- **Interactive Elements**: Click to select players and eggs
- **Visual Effects**: Particle systems, animations, and broadcast visualizations
- **Dragon Ball Z Theme**: Custom models and animations inspired by DBZ

## Architecture

### Core Components

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│    GUIApp       │    │      Map        │    │    Protocol     │
│  (Main App)     │────│   (Game World)  │    │ (Network Comm)  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │              ┌─────────────────┐              │
         └──────────────│      HUD        │──────────────┘
                        │ (User Interface)│
                        └─────────────────┘
```

### Class Hierarchy

- **GuiApp**: Main application controller
- **Map**: Game world management and rendering
- **Player**: Player entities with transformations
- **Egg**: Egg entities on the map
- **Protocol**: Network communication with server
- **Hud**: User interface and information display
- **BroadcastCircles**: Visual broadcast effects

## Installation and Setup

### Prerequisites
- C++17 compatible compiler
- Raylib graphics library
- POSIX compliant system (Linux/macOS)

### Build Instructions

```bash
# Build the GUI
make zappy_gui

# Run the GUI
./zappy_gui -p <port> -h <hostname>
```

### Command Line Arguments

```bash
USAGE: ./zappy_gui -p port -h machine
    -p port     Port number to connect to the server
    -h machine  Hostname of the server (default: localhost)
```

### Example
```bash
./zappy_gui -p 4242 -h localhost
```

## Usage

### Starting the Application
1. Ensure the Zappy server is running
2. Launch the GUI with appropriate connection parameters
3. The GUI will automatically connect and authenticate as "GRAPHIC"
4. The game world will be rendered in real-time

### Basic Navigation
- **Mouse Movement**: Look around (in free camera mode)
- **WASD Keys**: Move camera (in free camera mode)
- **Shift**: Toggle mouse cursor visibility
- **O Key**: Cycle through camera modes
- **Left Click**: Select players or eggs

## Features

### Camera Modes

#### 1. Free Camera (Default)
- Full 3D navigation
- Mouse look controls
- WASD movement
- Best for general observation

#### 2. Top-Down View
- Bird's eye perspective
- Fixed camera position above the map
- Good for strategic overview

#### 3. Player POV
- Follow selected player
- Third-person perspective
- Automatic camera tracking

### Visual Elements

#### Players
- **Dragon Ball Transformations**: Players appear as different Goku forms based on level
  - Level 1: Flying Goku
  - Level 2: Super Saiyan
  - Level 3: Super Saiyan 2
  - Level 4: Super Saiyan 3
  - Level 5: Super Saiyan 4
  - Level 6: Super Saiyan 5
  - Level 7: Super Saiyan God
  - Level 8: Super Saiyan Blue

- **Particle Effects**: Colored aura particles around players
- **Team Colors**: Each team has a unique color scheme

#### Resources
- **Food**: Senzu beans scattered across the map
- **Stones**: Dragon Balls (DB1-DB6) representing different minerals
  - DB1: Linemate
  - DB2: Deraumere
  - DB3: Sibur
  - DB4: Mendiane
  - DB5: Phiras
  - DB6: Thystame

#### Special Effects
- **Incantations**: Shenron (dragon) appears during elevation rituals
- **Broadcasts**: Purple expanding circles show communication
- **Eggs**: Golden eggs represent unhatched players

### User Interface

#### Resource Counter
- Displays total resources on the map
- Located in the top-left corner
- Real-time updates

#### Player Information Panel
- Shows selected player details:
  - Level
  - Inventory (food and stones)
  - Team affiliation
- POV toggle button

#### Egg Information Panel
- Shows team affiliation of selected eggs
- Appears when clicking on eggs

## Protocol Communication

### Server Connection
The GUI connects to the server using TCP sockets and follows the Zappy protocol:

```
Client → Server: GRAPHIC\n
Server → Client: WELCOME\n
Server → Client: [Game initialization data]
```

### Supported Commands

#### Map Information
- `msz X Y`: Map size
- `bct X Y q0 q1 q2 q3 q4 q5 q6`: Tile content
- `mct`: Request all tile contents

#### Player Events
- `pnw #n X Y O L N`: New player connection
- `ppo #n X Y O`: Player position update
- `plv #n L`: Player level update
- `pin #n X Y q0 q1 q2 q3 q4 q5 q6`: Player inventory
- `pdi #n`: Player death

#### Game Events
- `pbc #n M`: Broadcast message
- `pic X Y L #n #n ...`: Incantation start
- `pie X Y R`: Incantation end
- `pfk #n`: Player fork (lay egg)
- `pdr #n i`: Resource drop
- `pgt #n i`: Resource collection

#### Egg Events
- `enw #e #n X Y`: Egg laid
- `ebo #e`: Egg hatching
- `edi #e`: Egg death

#### System Events
- `seg N`: Game end
- `sgt T`: Time unit info
- `sst T`: Time unit change

## API Reference

### GUIApp Class
Main application controller managing the entire GUI.

```cpp
class GuiApp {
public:
    GuiApp(int windowWidth, int windowHeight, std::string host, int port, const char* title);
    void run();
    
private:
    void handleEvents();
    void render();
    void changeViewMode();
    void updatePlayerPOV();
};
```

### Map Class
Manages the game world state and rendering.

```cpp
class Map {
public:
    void initMapSize(float mapWidth, float mapHeight);
    void addPlayer(int x, int z, int playerId, float rotationAngle, const std::string &teamName, const Color& color);
    void addFood(int x, int y);
    void addStone(int x, int y, std::string stoneName);
    void draw();
    Player& getPlayerAtId(int id);
};
```

### Player Class
Represents individual players with transformations and effects.

```cpp
class Player {
public:
    Player(float x, float y, float rotation, std::vector<Model> transformationsModels,
           std::vector<Vector3> transformationsScales, int id, const std::string &teamName, const Color &color);
    void draw(Color baseColor);
    void levelUp();
    void setLevel(int level);
};
```

### Protocol Class
Handles network communication with the server.

```cpp
class Protocol {
public:
    Protocol(const std::string& host, int port);
    void sendCommand(const std::string& cmd);
    bool pollResponse(std::string& response);
    void start();
    void stop();
};
```

## File Structure

```
Gui/
├── main.cpp                 # Entry point
├── GUIApp/
│   ├── GUIApp.hpp          # Main application header
│   ├── GUIApp.cpp          # Main application implementation
│   └── CommandHandler.cpp  # Protocol command handling
├── Map/
│   ├── Map.hpp             # Game world header
│   └── Map.cpp             # Game world implementation
├── Player/
│   ├── Player.hpp          # Player entity header
│   └── Player.cpp          # Player entity implementation
├── Egg/
│   ├── Egg.hpp             # Egg entity header
│   └── Egg.cpp             # Egg entity implementation
├── Protocol/
│   ├── Protocol.hpp        # Network communication header
│   └── Protocol.cpp        # Network communication implementation
├── Hud/
│   ├── Hud.hpp             # UI header
│   └── Hud.cpp             # UI implementation
└── BroadcastCircle/
    └── BroadcastCircle.hpp # Broadcast effects
```

## Controls

### Camera Controls
| Key/Action | Description |
|------------|-------------|
| Mouse Movement | Look around (free camera) |
| W,A,S,D | Move camera (free camera) |
| Shift (Hold) | Show/hide mouse cursor |
| C | Cycle camera modes |

### Interaction Controls
| Key/Action | Description |
|------------|-------------|
| Left Click | Select player/egg |
| POV Button | Enter/exit player POV mode |


## Troubleshooting

### Connection Issues
**Problem**: GUI fails to connect to server
- Verify server is running and accessible
- Check port number and hostname
- Ensure firewall settings allow connection

**Problem**: "Connection refused" error
- Confirm server is listening on specified port
- Check if another GUI client is already connected

### Rendering Issues
**Problem**: Models not displaying correctly
- Verify all asset files are present in `assets/` directory
- Check file permissions for asset files
- Ensure Raylib is properly installed

**Problem**: Low frame rate
- Reduce window size
- Check system graphics capabilities
- Monitor resource usage

### Protocol Issues
**Problem**: Commands not working
- Verify server protocol compatibility
- Check network connectivity
- Review server logs for errors

### Common Error Messages
- `"Error: Couldn't handle command X, no player found with ID: Y"`: Player data inconsistency
- `"Error: Couldn't handle command X, no egg found with ID: Y"`: Egg data inconsistency
- `"Failed to connect to server"`: Network or server issues