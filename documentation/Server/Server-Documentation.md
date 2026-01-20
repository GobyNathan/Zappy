# Zappy Server Documentation

## Table of Contents
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Installation and Usage](#installation-and-usage)
4. [Core Components](#core-components)
5. [Network Protocol](#network-protocol)
6. [Game Mechanics](#game-mechanics)
7. [Server Commands](#server-commands)
8. [API Reference](#api-reference)

## Overview

The Zappy server is a network game server written in C that manages a virtual world called Trantor. It handles multiple AI clients and graphical clients simultaneously, managing resources, player actions, and game state in real-time.

### Key Features
- Multi-client support with socket multiplexing using `poll()`
- Real-time game simulation with configurable time units
- Support for both AI players and graphical observers
- Resource management and spawning system
- Team-based gameplay with elevation mechanics

## Architecture

### Main Components

```
zappy_server/
├── Server Core
│   ├── main.c                 # Entry point
│   ├── run_server.c           # Main server loop
│   └── server_info.c          # Server configuration
├── Client Management
│   ├── client_utilities.c     # Client connection handling
│   ├── manage_client.c        # Client input processing
│   └── network_utilities.c    # Socket operations
├── Game Logic
│   ├── map_utilities.c        # Map operations
│   ├── individual_utilities.c # Player management
│   ├── action_queue.c         # Action scheduling
│   └── food_handling.c        # Resource management
├── AI Protocol
│   ├── handle_ai_client.c     # AI command processing
│   ├── movement.c             # Movement commands
│   ├── inventory.c            # Inventory management
│   ├── look.c                 # Vision system
│   ├── objects.c              # Object manipulation
│   ├── broadcast.c            # Communication system
│   ├── elevation.c            # Level progression
│   ├── fork.c                 # Reproduction
│   └── ejection.c             # Player ejection
└── GUI Protocol
    ├── handle_gui_client.c    # GUI command processing
    ├── msz.c, bct.c, mct.c   # Map commands
    ├── pnw.c, ppo.c, plv.c   # Player commands
    └── Various notification handlers
```

### Server Flow

1. **Initialization**
   - Parse command line arguments
   - Create server socket
   - Initialize game map
   - Spawn initial resources and eggs

2. **Main Loop**
   - Poll for socket events
   - Accept new connections
   - Process client commands
   - Execute scheduled actions
   - Update game state
   - Send notifications to GUI clients

3. **Time Management**
   - Actions are scheduled based on game time units
   - Formula: `execution_time = action_time / frequency`
   - Default frequency: 100

## Installation and Usage

### Compilation

```bash
make zappy_server
```

### Running the Server

```bash
./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
```

#### Parameters

| Option | Description | Example |
|--------|-------------|---------|
| `-p port` | Port number for the server | `-p 4242` |
| `-x width` | Width of the game world | `-x 10` |
| `-y height` | Height of the game world | `-y 10` |
| `-n names...` | Team names (space-separated) | `-n team1 team2` |
| `-c clientsNb` | Initial slots per team | `-c 3` |
| `-f freq` | Time unit frequency | `-f 100` |

### Example

```bash
./zappy_server -p 4242 -x 20 -y 20 -n Alpha Beta Gamma -c 5 -f 100
```

## Core Components

### Map System

The game world is a toroidal (wrap-around) grid where:
- Players exiting one edge appear on the opposite edge
- Each tile can contain multiple elements (resources, players)
- Resources spawn randomly according to density formulas

#### Resource Densities

| Resource | Density | Formula |
|----------|---------|---------|
| Food | 0.5 | `map_width × map_height × 0.5` |
| Linemate | 0.3 | `map_width × map_height × 0.3` |
| Deraumere | 0.15 | `map_width × map_height × 0.15` |
| Sibur | 0.1 | `map_width × map_height × 0.1` |
| Mendiane | 0.1 | `map_width × map_height × 0.1` |
| Phiras | 0.08 | `map_width × map_height × 0.08` |
| Thystame | 0.05 | `map_width × map_height × 0.05` |

### Client Management

The server supports three types of clients:

1. **Unassigned Clients**: Newly connected, awaiting team assignment
2. **AI Clients**: Control individual players
3. **GUI Clients**: Observe the game state

#### Connection Flow

1. Client connects to server
2. Server sends: `WELCOME\n`
3. Client sends team name or `GRAPHIC`
4. Server responds with game information

### Action Queue System

Actions are scheduled and executed based on game ticks:

```c
typedef struct scheduled_action_s {
    client_t *client;
    callback_info_t *info;
    struct scheduled_action_s *next;
} scheduled_action_t;
```

Actions are inserted in chronological order and executed when their scheduled tick is reached.

## Network Protocol

### AI Client Protocol

#### Connection
```
<-- WELCOME\n
--> TEAM-NAME\n
<-- CLIENT-NUM\n
<-- X Y\n
```

#### Commands

| Command | Description | Time | Response |
|---------|-------------|------|----------|
| `Forward` | Move forward one tile | 7/f | `ok` |
| `Right` | Turn 90° right | 7/f | `ok` |
| `Left` | Turn 90° left | 7/f | `ok` |
| `Look` | Look around | 7/f | `[tile1, tile2,...]` |
| `Inventory` | Check inventory | 1/f | `[linemate n, sibur n,...]` |
| `Broadcast text` | Broadcast message | 7/f | `ok` |
| `Connect_nbr` | Get available slots | - | number |
| `Fork` | Lay an egg | 42/f | `ok` |
| `Eject` | Eject players from tile | 7/f | `ok/ko` |
| `Take object` | Pick up object | 7/f | `ok/ko` |
| `Set object` | Drop object | 7/f | `ok/ko` |
| `Incantation` | Start elevation ritual | 300/f | `Elevation underway/ko` |

### GUI Client Protocol

GUI clients authenticate with `GRAPHIC` as team name.

#### Map Commands
- `msz` - Get map size
- `bct X Y` - Get tile content
- `mct` - Get all tiles content

#### Player Commands
- `ppo #n` - Get player position
- `plv #n` - Get player level
- `pin #n` - Get player inventory

#### Time Commands
- `sgt` - Get time unit
- `sst T` - Set time unit

#### Event Notifications
- `pnw` - New player connection
- `pex` - Player expulsion
- `pbc` - Broadcast
- `pic` - Incantation start
- `pie` - Incantation end
- `pfk` - Fork (egg laying)
- `pdr` - Resource drop
- `pgt` - Resource collection
- `pdi` - Player death
- `enw` - Egg laid
- `ebo` - Egg hatched
- `seg` - End of game

## Game Mechanics

### Player Stats
- Initial food: 10 units (1260 time units)
- Initial level: 1
- Vision range increases with level

### Elevation Requirements

| Level | Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|---------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

### Vision System

Players see tiles in a cone in front of them:
- Level 1: 3 tiles (1 ahead)
- Level 2: 8 tiles (2 ahead)
- Level 3: 15 tiles (3 ahead)
- And so on...

### Broadcast System

Messages propagate with directional information:
- Tile 0: Same location
- Tiles 1-8: Directional quadrants

## Server Commands

The server accepts commands via standard input:

| Command | Description |
|---------|-------------|
| `/clients` | List all connected clients |
| `/quit` | Stop the server |
| `/send_ais msg` | Send message to all AI clients |
| `/send_guis msg` | Send message to all GUI clients |
| `/map` | Display map information |
| `/clear` | Clear the shell |
| `/pause` | Pause AI actions |
| `/start` | Start the server |
| `/setTile resource quantity x y` | Modify tile resources |
| `/tile x y` | Get tile inventory |
| `/tp id x y` | Teleport player |
| `/kill id` | Kill player |
| `/noFood true/false` | Toggle food management |
| `/broadcast "message" x y` | Simulate broadcast |
| `/setLevel id level` | Set player level |
| `/setInventory id resource quantity` | Modify player inventory |
| `/setClientsNb nb` | Set minimum clients per team |
| `/setFreq freq` | Change game frequency |
| `/noRefill true/false` | Toggle map refill |
| `/fork team x y` | Simulate fork |
| `/incantate x y` | Simulate incantation |

## API Reference

### Key Structures

#### server_info_t
```c
typedef struct server_info_s {
    int port;
    char **names;
    int original_slots_nb;
    int freq;
    map_info_t *map;
    individual_t **individuals;
    int current_tick;
    scheduled_action_t *action_queue;
} server_info_t;
```

#### client_t
```c
typedef struct client_s {
    int sockfd;
    individual_t *individual;
    char *team;
    char *buffer;
} client_t;
```

#### individual_t
```c
typedef struct individual_s {
    int player_id;
    int food_lvl;
    int main_lvl;
    int vision_range;
    int pos_x;
    int pos_y;
    lookat_t look_at;
    bool is_alive;
    bool is_egg;
    inventory_t *inventory;
    char *team;
    int *elevation_group;
    int elevation_group_size;
} individual_t;
```

### Key Functions

#### Server Management
- `run_server(server_info_t *info)` - Main server loop
- `create_server_socket(int port)` - Create and bind server socket
- `accept_client(clients_management_t *client_manage)` - Accept new connections

#### Client Management
- `manage_client_input(server_info_t *info, clients_management_t *client_manage, int client_index)` - Process client input
- `remove_client(clients_management_t *client_manage, int client_index)` - Disconnect client
- `send_msg_client(client_t *client, const char *msg)` - Send message to client

#### Game Logic
- `tick_action_queue(scheduled_action_t **head, int current_tick)` - Execute scheduled actions
- `refresh_map(map_info_t *map)` - Spawn new resources
- `decrease_food_players(server_info_t *info, clients_management_t *client_manage)` - Update player food levels

## Error Handling

The server includes robust error handling for:
- Invalid command parameters (`sbp` response)
- Unknown commands (`suc` response)
- Failed actions (`ko` response)
- Network errors (automatic client disconnection)

## Performance Considerations

- Single-threaded design using `poll()` for efficiency
- Action queue prevents blocking operations
- Buffered client input (up to 10 commands)
- Optimized map operations with modulo arithmetic for wrapping