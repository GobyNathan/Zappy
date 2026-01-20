# Zappy AI Documentation

## Table of Contents
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Core Components](#core-components)
4. [AI Strategy](#ai-strategy)
5. [Communication Protocol](#communication-protocol)
6. [Installation and Usage](#installation-and-usage)
7. [Configuration](#configuration)
8. [API Reference](#api-reference)

## Overview

The Zappy AI is an autonomous client that controls a player (Trantorian) in the Zappy game world. The AI's objective is to help its team reach the winning condition: having at least 6 players reach the maximum elevation level (level 8).

### Key Features
- **Autonomous gameplay** with no user intervention required
- **Multi-process architecture** with intelligent forking strategy
- **Resource management** and strategic collection
- **Team coordination** through broadcast communication
- **Adaptive decision-making** based on game state

## Architecture

The AI follows a modular architecture with clear separation of concerns:

```
┌─────────────────┐
│     main.py     │ ← Entry point
└─────────────────┘
         │
┌─────────────────┐
│    zappy.py     │ ← Main controller
└─────────────────┘
         │
┌─────────────────┬─────────────────┬─────────────────┐
│   client.py     │    game.py      │ fork_manager.py │
│   (Network)     │ (Game Logic)    │  (Process Mgmt) │
└─────────────────┴─────────────────┴─────────────────┘
         │                 │
┌─────────────────┐ ┌─────────────────┐
│   settings.py   │ │    Protocol     │
│ (Configuration) │ │   Definitions   │
└─────────────────┘ └─────────────────┘
```

## Core Components

### 1. Zappy (`zappy.py`)
Main AI controller that coordinates all components.

**Key Responsibilities:**
- Server connection management
- Response handling and parsing
- Main execution loop
- Command coordination

### 2. NetworkClient (`client.py`)
Handles TCP communication with the Zappy server.

**Features:**
- **Asynchronous communication** with separate send/receive threads
- **Command queuing** (max 10 pending commands as per protocol)
- **Automatic reconnection** handling
- **Thread-safe operations**

**Key Methods:**
```python
def connect() -> bool                    # Connect to server
def send_command(command: str) -> bool   # Send command (non-blocking)
def get_response(timeout: float) -> str  # Get server response
def disconnect()                         # Clean shutdown
```

### 3. Game (`game.py`)
Core game logic and AI decision-making engine.

**State Management:**
- Player position and orientation
- Inventory and food levels
- World map representation
- Team coordination state

**Key Attributes:**
```python
self.level: int                    # Current player level (1-8)
self.food: int                     # Food units (survival resource)
self.position: List[int]           # [y, x] coordinates
self.orientation: str              # N, S, E, W
self.inventory: Dict[str, int]     # Resource quantities
self.map: List[List[str]]          # World map representation
```

### 4. ForkManager (`fork_manager.py`)
Manages AI process spawning and team size control.

**Features:**
- **Process limit management** with file-based synchronization
- **Automatic spawning** of new AI instances
- **Resource cleanup** for terminated processes
- **Team coordination** through shared state files

## AI Strategy

### Decision Priority System

The AI uses a priority-based decision system executed in `make_decision()`:

1. **Survival Priority** - Near-death food management
2. **Reproduction Priority** - Forking when conditions are met
3. **Incantation Priority** - Level advancement opportunities
4. **Resource Collection** - Strategic resource gathering
5. **Exploration** - Map discovery and movement

### Forking Strategy

The AI implements an intelligent forking strategy:

```python
def can_fork(self, food: int) -> bool:
    """Check if the condition to fork are met"""
    if self.is_first_process and food >= 18 and self.max_fork >= 1 and not self.fork_in_progress:
        return True
    return False
```

**Forking Rules:**
- Only the first process in a team can initiate forks
- Requires minimum food level (18 units)
- Respects team size limits
- Spawns new processes automatically if `auto_join` is enabled

### Incantation System

The AI coordinates complex incantation rituals requiring multiple players:

**Requirements per Level:**
```python
INCANTATION_REQUIREMENTS = {
    2: {'players': 1, 'resources': {'linemate': 1}},
    3: {'players': 2, 'resources': {'linemate': 1, 'deraumere': 1, 'sibur': 1}},
    4: {'players': 2, 'resources': {'linemate': 2, 'sibur': 1, 'phiras': 2}},
    5: {'players': 4, 'resources': {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'phiras': 1}},
    6: {'players': 4, 'resources': {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3}},
    7: {'players': 6, 'resources': {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'phiras': 1}},
    8: {'players': 6, 'resources': {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1}}
}
```

**Incantation Process:**
1. **Resource Verification** - Check inventory requirements
2. **Player Coordination** - Broadcast to gather required players
3. **Site Preparation** - Drop resources on ground
4. **Ritual Execution** - Perform incantation command
5. **Result Handling** - Process success/failure

### Resource Management

**Resource Priority Weighting:**
```python
def _get_resource_priority_weight(self, resource: str) -> float:
    weights = {
        'food': 1.0,
        'linemate': 2.0,
        'deraumere': 3.0,
        'sibur': 4.0,
        'mendiane': 4.0,
        'phiras': 5.0,
        'thystame': 6.0
    }
    return weights.get(resource, 1.0)
```

**Collection Strategy:**
- **Food Priority** - Always prioritize survival
- **Level-based Collection** - Focus on next level requirements
- **Rarity Consideration** - Prioritize scarcer resources
- **Pathfinding** - Use BFS to find closest resources

### Team Communication

The AI uses broadcast messages for team coordination:

**Message Types:**
- `NewCharacter_<id>_Level_<level>` - Announce new team member
- `WantPlayer` - Request players for incantation
- `IAmHere_<id>` / `IAmNotHere_<id>` - Position confirmation
- `StartIncantation` / `EndOfIncantation` - Ritual coordination
- `Death_<id>` - Announce player death

## Communication Protocol

### Server Connection Process

1. **Socket Connection** to server port
2. **Handshake Sequence:**
   ```
   Server -> WELCOME\n
   Client -> TEAM_NAME\n
   Server -> CLIENT_NUM\n
   Server -> X Y\n  (world dimensions)
   ```

### Command Protocol

**Available Commands:**
| Command | Time Limit | Response | Description |
|---------|------------|----------|-------------|
| `Forward` | 7/f | `ok` | Move one tile forward |
| `Right` | 7/f | `ok` | Turn 90° right |
| `Left` | 7/f | `ok` | Turn 90° left |
| `Look` | 7/f | `[tile1, tile2, ...]` | Get vision data |
| `Inventory` | 1/f | `[resource n, ...]` | Get inventory |
| `Broadcast text` | 7/f | `ok` | Send team message |
| `Connect_nbr` | - | `value` | Get available team slots |
| `Fork` | 42/f | `ok` | Create new player |
| `Eject` | 7/f | `ok/ko` | Eject players from tile |
| `Take object` | 7/f | `ok/ko` | Pick up resource |
| `Set object` | 7/f | `ok/ko` | Drop resource |
| `Incantation` | 300/f | `Elevation underway\|Current level: k/ko` | Start level ritual |

**Command Queue Rules:**
- Maximum 10 pending commands
- Commands executed in FIFO order
- Excess commands are dropped

## Installation and Usage

### Requirements
- Python 3.7+
- Standard library modules only (no external dependencies)

### Basic Usage

```bash
# Start a single AI
./zappy_ai -p <port> -n <team_name> [-m <hostname>] [-a]

# Example
./zappy_ai -p 4242 -n team1 -m localhost -a
```

### Command Line Arguments

| Argument | Required | Description | Default |
|----------|----------|-------------|---------|
| `-p, --port` | Yes | Server port number | - |
| `-n, --name` | Yes | Team name | - |
| `-m, --machine` | No | Server hostname | `localhost` |
| `-a, --auto-join` | No | Enable automatic forking | `False` |

## Configuration

### Game Settings (`settings.py`)

The main configuration file contains incantation requirements:

```python
INCANTATION_REQUIREMENTS = {
    # level: {'players': count, 'resources': {resource: quantity}}
    2: {'players': 1, 'resources': {'linemate': 1}},
    # ... additional levels
}
```

### Fork Manager Settings

```python
DEFAULT_MAX_FORK = 7      # Maximum processes per team
MAX_FORK_FILE = "/tmp/max_fork_"    # Fork limit file prefix
FORK_LOCK_FILE = "/tmp/fork_"       # Fork synchronization file prefix
```

### Tunable Parameters

**In `game.py`:**
```python
min_food = 8              # Minimum food for incantation participation
self.food <= 10           # Critical food threshold
self.food <= 20           # Low food threshold
food >= 18                # Fork food requirement
```

## API Reference

### Main Classes

#### Zappy
```python
class Zappy:
    def __init__(self, hostname: str, port: int, team_name: str, auto_join: bool)
    def connect(self) -> bool
    def run(self)
    def handle_response(self, response: str)
```

#### NetworkClient
```python
class NetworkClient:
    def __init__(self, hostname: str, port: int, team_name: str, game: 'Game')
    def connect(self) -> bool
    def send_command(self, command: str) -> bool
    def get_response(self, timeout: float = 0.1) -> Optional[str]
    def disconnect(self)
```

#### Game
```python
class Game:
    def __init__(self, map_width: int, map_height: int, zappy: 'Zappy', auto_join: bool)
    def make_decision(self)
    def can_incantate(self) -> bool
    def parse_vision(self, view: str)
    def parse_inventory(self, inv: str)
    # ... additional methods
```

#### ForkManager
```python
class ForkManager:
    def __init__(self, map_width: int, map_height: int, team_name: str, player_id: int)
    def can_fork(self, food: int) -> bool
    def try_fork(self, zappy_port: int, team_name: str, auto_join: bool)
    def shutdown_all_forks(self)
```

### Key Algorithms

#### Pathfinding (BFS)
```python
def _find_closest_ressource(self, ressource: str) -> List:
    """Return the path to the closest resource using BFS"""
    # Implements breadth-first search with toroidal world wrapping
```

#### Vision Parsing
```python
def parse_vision(self, view: str):
    """Parse look command response and update world map"""
    # Converts relative vision data to absolute map coordinates
```

#### Decision Engine
```python
def make_decision(self):
    """Main AI decision-making logic with priority system"""
    # Implements the priority-based strategy described above
```

### Error Handling

The AI implements robust error handling:

- **Network Failures** - Automatic reconnection attempts
- **Command Failures** - State rollback and retry logic  
- **Process Management** - Cleanup of dead fork processes
- **Resource Conflicts** - Coordination through broadcasts
- **Timeout Handling** - Non-blocking operations with timeouts

### Logging and Debugging

**Console Output:**
- Command execution status
- Decision reasoning
- Broadcast message handling
- Error conditions and recovery

**Process Logs:**
- Each forked process creates individual log files in `logs/` directory
- Format: `ai_{team_name}_{parent_id}_{random_id}.log`

This AI implementation provides a sophisticated, autonomous player capable of coordinating with teammates to achieve the game's victory conditions through intelligent resource management, strategic positioning, and coordinated incantation rituals.