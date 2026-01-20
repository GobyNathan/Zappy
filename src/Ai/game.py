from typing import List, Dict, TYPE_CHECKING
from collections import deque
from src.Ai.settings import INCANTATION_REQUIREMENTS
from src.Ai.fork_manager import ForkManager
import random

if TYPE_CHECKING:
    from .zappy import Zappy

class Game:
    """Game info"""

    def __init__(self, map_width: int, map_height: int, zappy: 'Zappy', auto_join: bool):
        self.map_width = map_width
        self.map_height = map_height
        self.auto_join = auto_join

        self.zappy = zappy
        self.last_look = ""

        self.level = 1
        self.food = 10
        self.position = [map_height // 2, map_width // 2]
        self.orientation = "N"

        self.map: List[List[str]] = [[ "" for _ in range(map_width)] for _ in range(map_height)]

        self.inventory = {
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0
        }

        self.player_id = random.randint(0, 500)
        
        # Player_id: player_level
        self.players_info : Dict[int, int]= {}

        # Player_id: player_is_here
        self.tot_player_here: Dict[int, bool] = {}

        self.send_look = False
        self.fork_manager = ForkManager(map_width, map_height, self.zappy.team_name, self.player_id)

        self.has_incantation_objective = False
        self.incantation_objective_path = []

        # Incantation state
        self.is_incanting = False
        self.current_target_resource = None

        self.character_counter = 1  # Track total team characters (starts at 1 for this AI)
        self.my_fork_count = 0  # Track how many times THIS AI has forked
        self.decision_counter = 0  # Track decisions for command-based intervals

        self.wanted_incantation = False # If player want to incantate
        self.has_received_incantation_broadcast = False
        self.has_arrived_to_incantation = False # If player has arrived to incantation broadcast

        self.can_move = True # Check if player is needed in an incantation process

        self.has_said_not_here = False # Bool to store if player has said 'IAmNotHere', avoid to send it multiple time

        self.already_dropped = {
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0
        }

        self.tot_all_player = 8 # Max of player per team

        self.last_incantation = 0 # Store the last incantation timing based on decision_counter

    def make_decision(self):
        """Main part of the ai strategy"""
        
        # Increment decision counter for command-based intervals
        self.decision_counter += 1

        # Check player level's at regular interval
        if self.decision_counter % 100 == 1:
            self.check_all_player_level()

        if not self.can_move:
            print("[can't move]")
            self.zappy.client.clear_command_queue()
            return

        # Near death experience
        if self.food <= 1 and self.character_counter > 1:
            print("[death_experience]")
            self.death_experience()
            return
        
        # Fork strategy : fork until we reach max number of player
        if self.fork_manager.can_fork(self.food, self.character_counter):
            print("[fork_strategy]")
            self.zappy.client.send_command("Fork")
            return

        min_food = 8
        # Check if the player has arrived to the destination
        if self.has_received_incantation_broadcast and self.has_arrived_to_incantation and self.food > min_food:
            print("[Player arrived to dest - don't move]")
            self.zappy.client.clear_command_queue()
            return

        # Check if we wanted to incantate so that it gather all necessary players
        if self.wanted_incantation and self.food > 10:
            print("[wanted to incantate]")
            # Check if condition are still true
            print("Dict:", self.tot_player_here)
            if not self.can_incantate() and self.already_droped_is_empty():
                self.wanted_incantation = False
            elif self.all_player_are_here():
                print("ALL PLAYER ARE HERE")
                self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-StartIncantation")
                if not self.prepare_incantation():
                    print("Failed to prepare incantation. Waiting for next time")
                    return
                self.is_incanting = True
                self.last_incantation = self.decision_counter
                self.zappy.client.send_command("Incantation")
            elif self.decision_counter % 2 == 0:
                self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-WantPlayer")
            return

        # Go for resources
        tile = self.map[self.position[0]][self.position[1]]
        
        # Priority 1: Critical food situation
        if self.food <= 10:
            print("[look_for_food]")
            self.look_for_food()
            self.wanted_incantation = False
            return

        # Priority 2: Check for incantation opportunity (but only if we have enough food)
        if self.food >= 12:
            print("[check can_incantate]")
            if self.can_incantate():
                print("[can_incantate]")
                print(f"Ready for incantation to level {self.level + 1}!")
                self.wanted_incantation = True
                return
            else:
                self.wanted_incantation = False
            
        # Priority 3: Collect resources on current tile ONLY for first process
        if self.fork_manager.is_first_process and tile.count("_") == 0 and tile != "":
            print("[Pick up on tile]")
            # Resource on the tile
            ressource = tile.strip().split(' ')[0]
            self.zappy.client.send_command("Take " + ressource.strip())
            self.take_object(ressource.strip())
            return
            
        # # Priority 4: Look for food if getting low for EVERYONE
        if self.food <= 20:
            print("[look_for_food > 15]")
            self.look_for_food()
            return
            
        # Priority 5: Strategic resource collection ONLY for first process
        if self.fork_manager.is_first_process:
            needed_resource = self.get_most_needed_resource()
            if needed_resource and self.search_for_resource(needed_resource):
                print("[get_most_needed_resource]")
                return
            
        # Priority 6: Random exploration
        print("[random_exploration]")
        self.random_exploration()

    def check_all_player_level(self):
        all_level = {}
        # Count number of level
        for level in self.players_info.values():
            all_level[level] = all_level.get(level, 0) + 1
        
        # Check if a player doesn't have the same level as the other
        if len(all_level) == 2:
            if all_level[min(all_level)] == 1 and min(all_level) == self.level:
                print("Player is late in level. Suicide.")
                self.death_experience()

    def reset_already_droped(self):
        self.already_dropped = {key: 0 for key in self.already_dropped}

    def already_droped_is_empty(self):
        all(value == 0 for value in self.already_dropped.values())

    def drop_all_inventory(self):
        """Drop all inventory resources"""
        print(f"Drop full inventory.")


        order = list(self.inventory.keys())
        while any(value > 0 for value in self.inventory.values()):
            for key in order:
                if self.inventory[key] > 0:
                    self.inventory[key] -= 1
                    self.zappy.client.send_command("Set " + key)

    def drop_all_food(self):
        """Drop all food"""
        for _ in range(self.food):
            self.zappy.client.send_command("Set food")

    def death_experience(self):
        """Will drop all resources and communicate to others"""
        if self.fork_manager.is_first_process:
            if len(self.players_info) >= 1:
                self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-FirstProcess_" + next(iter(self.players_info)))
        self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-Death_" + str(self.player_id))
        self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-Death_" + str(self.player_id))
        self.drop_all_inventory()
        self.drop_all_food()

    def can_incantate(self) -> bool:
        """Check if current level incantation is possible"""
        if not self.fork_manager.is_first_process:
            return False

        if self.has_received_incantation_broadcast:
            return False

        if self.character_counter < self.tot_all_player:
            return False
        
        if self.level >= 8:
            return False
        
        # Avoid incanting 2 time in a row (must have a 20 decision interval)
        if self.decision_counter - self.last_incantation < 20:
            return False
            
        next_level = self.level + 1
        if next_level not in INCANTATION_REQUIREMENTS:
            return False
            
        requirements = INCANTATION_REQUIREMENTS[next_level]
        
        # Check resource requirements in inventory
        for resource, needed in requirements['resources'].items():
            if self.inventory.get(resource, 0) < needed:
                return False
        
        print("READY FOR INCANTATION TO LEVEL", next_level)
        return True

    def _drop_necessary_resources(self, next_level, reversed_mode):
        """Drop on the ground the necesary resource for given level. Drop in reverse mode if specified"""
        if next_level not in INCANTATION_REQUIREMENTS:
            return

        requirements = INCANTATION_REQUIREMENTS[next_level]

        # Reversed mode
        if reversed_mode:
            for resource, needed in reversed(list(requirements['resources'].items())):
                already = self.already_dropped.get(resource, 0)
                remaining_to_drop = needed - already

                if remaining_to_drop <= 0:
                    continue  # Already dropped enough of this resource

                inventory_count = self.inventory.get(resource, 0)

                if inventory_count >= remaining_to_drop:
                    for _ in range(remaining_to_drop):
                        print(f"Dropping {resource} on ground")
                        if self.zappy.client.send_command(f"Set {resource}"):
                            self.already_dropped[resource] = self.already_dropped.get(resource, 0) + 1
                            self.inventory[resource] -= 1
                        else:
                            return False
                else:
                    return False 
        
        # Normal mode
        for resource, needed in requirements['resources'].items():
            already = self.already_dropped.get(resource, 0)
            remaining_to_drop = needed - already

            if remaining_to_drop <= 0:
                continue  # Already dropped enough of this resource

            inventory_count = self.inventory.get(resource, 0)

            if inventory_count >= remaining_to_drop:
                for _ in range(remaining_to_drop):
                    print(f"Dropping {resource} on ground")
                    if self.zappy.client.send_command(f"Set {resource}"):
                        self.already_dropped[resource] = self.already_dropped.get(resource, 0) + 1
                        self.inventory[resource] -= 1
                    else:
                        return False
            else:
                return False
        
        return True

    def prepare_incantation(self) -> bool:
        """Drop required resources on ground and attempt incantation"""
        next_level = self.level + 1

        print(f"Preparing incantation site for level {next_level}")
        return self._drop_necessary_resources(next_level, False)

    def get_most_needed_resource(self) -> str:
        """Determine which resource we need most for next incantation"""
        
        # Special case: Solo player at level 1-2 should prioritize level 3 resources
        if self.level <= 2 and self.character_counter == 1:
            level3_missing = self.get_missing_resources_for_level(3)
            if level3_missing:
                # Prioritize rarest resources first
                priority_order = ['sibur', 'deraumere', 'linemate']  # sibur and deraumere are rarer
                for resource in priority_order:
                    if resource in level3_missing:
                        print(f"Solo strategy: prioritizing {resource} for level 3 (need {level3_missing[resource]} more)")
                        return resource
        
        # Normal strategy: focus on current level requirements
        if self.level >= 8:
            return None
            
        next_level = self.level + 1
        if next_level not in INCANTATION_REQUIREMENTS:
            return None
            
        requirements = INCANTATION_REQUIREMENTS[next_level]['resources']
        
        # Find resource we need most urgently
        max_shortage = 0
        most_needed = None
        
        for resource, needed in requirements.items():
            current = self.inventory.get(resource, 0)
            shortage = needed - current
            
            if shortage > 0:
                # Priority weighting
                weight = self._get_resource_priority_weight(resource)
                weighted_shortage = shortage * weight
                
                if weighted_shortage > max_shortage:
                    max_shortage = weighted_shortage
                    most_needed = resource
                    
        if most_needed:
            print(f"Most needed resource: {most_needed} (shortage: {needed - self.inventory.get(most_needed, 0)})")
            
        return most_needed

    def _get_resource_priority_weight(self, resource: str) -> float:
        """Get priority weight for resources (rarer = higher priority)"""
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

    def search_for_resource(self, target_resource: str) -> bool:
        """Search for specific resource and move towards it"""
        self.current_target_resource = target_resource
        
        if not self.send_look and not self.zappy.client.send_command("Look"):
            return False
        self.send_look = True

        # Get a response aggressively
        for _ in range(10):
            response = self.zappy.client.get_response(timeout=0.1)
            if response:
                self.zappy.handle_response(response)
                if self.zappy.is_look_response(response):
                    self.last_look = response
                    break
        
        if self.last_look == "":
            return False

        if self.has_arrived_to_incantation:
            return False

        self.send_look = False
        self.last_look = ""

        path = self._find_closest_ressource(target_resource)

        if not path:
            print(f"No {target_resource} found nearby, exploring...")
            self.random_exploration()
            return False

        action = self._determine_next_action(path, target_resource)
        if action:
            self.zappy.client.send_command(action)
            return True
        return False

    def random_exploration(self):
        """Enhanced random exploration with resource awareness"""
        # If we have a target resource, bias towards looking
        if self.current_target_resource:
            actions = ["Forward", "Forward", "Look", "Right", "Left"]
        else:
            actions = ["Forward", "Forward", "Forward", "Right", "Left"]
            
        action = random.choice(actions)
        self.zappy.client.send_command(action)

    def look_for_food(self):
        if not self.send_look and not self.zappy.client.send_command("Look"):
            return
        self.send_look = True

        # Get a response aggressively
        for _ in range(10):
            response = self.zappy.client.get_response(timeout=0.1)
            if response:
                self.zappy.handle_response(response)
                if self.zappy.is_look_response(response):
                    self.last_look = response
                    break
        if self.has_arrived_to_incantation or self.has_received_incantation_broadcast:
            return
        
        self.send_look = False
        if self.last_look == "":
            return

        path = self._find_closest_ressource("food")

        if not path:
            self.random_exploration()
            return

        self._determine_next_action(path, "food")
        self.last_look = ""

    def _determine_next_action(self, path: List, resource_type: str | None) -> str:
        """Given a path to a ressource (type). If `ressource_type` is None it wont send `Take object` command."""
        if not path:
            return None

        DIRECTIONS = ['N', 'E', 'S', 'W']
        instructions = []
        current_pos = self.position
        orientation = self.orientation

        for next_pos in path:
            target_dir = direction_to(current_pos[0], current_pos[1], next_pos[0], next_pos[1], self.map_height, self.map_width)
            if not target_dir:
                continue  # already there

            current_idx = DIRECTIONS.index(orientation)
            target_idx = DIRECTIONS.index(target_dir)

            # Compute shortest rotation
            diff = (target_idx - current_idx) % 4
            
            if diff == 0:
                instructions.append("Forward")
            elif diff == 1:
                instructions.append("Right")
                instructions.append("Forward")
            elif diff == 2:
                instructions.append("Right")
                instructions.append("Right")
                instructions.append("Forward")
            elif diff == 3:
                instructions.append("Left")
                instructions.append("Forward")

            orientation = target_dir
            current_pos = next_pos

        if resource_type:
            instructions.append("Take " + resource_type.strip())
            self.take_object(resource_type.strip())

        print("list:", instructions, "last tile:", self.map[path[-1][0]][path[-1][1]])

        for cmd in instructions:
            if not cmd:
                break
            self.zappy.client.send_command(cmd)
        
        path.clear()

    def get_relative_tile(self, direction: int) -> List[int]:
        """
        Given relative direction (1-8),
        return the map coordinates of the tile in that direction.
        """
        y, x = self.position

        rel_offsets = [
            (-1, 0),   # 1: forward
            (-1, -1),  # 2
            (0, -1),   # 3: left
            (1, -1),   # 4
            (1, 0),    # 5: backward
            (1, 1),    # 6
            (0, 1),    # 7: right
            (-1, 1),   # 8
        ]

        # Orientation maps to how we rotate the relative directions
        orientation_rotation = {
            'N': 0,   # No rotation
            'E': 2,   # Rotate 90°, 2 steps
            'S': 4,   # Rotate 180°, 4 steps
            'W': 6    # Rotate 270°, 6 steps
        }

        # Apply the rotation to align base_directions with current orientation
        rotation_steps = orientation_rotation[self.orientation[0]]


        rotated_directions = rel_offsets[-rotation_steps:] + rel_offsets[:-rotation_steps]
        # Direction is 1-based index (1 to 8), so we subtract 1 to get the correct offset
        dy, dx = rotated_directions[direction - 1]
    
        return [y + dy, x + dx]

    def _get_neighbors(self, row, col, total_row, total_col) -> List:
        moves = []

        # Up
        next_row = row - 1
        if next_row < 0:
            moves.append(((total_row - 1, col),))
        else:
            moves.append(((next_row, col),))

        # Down
        next_row = row + 1
        if next_row >= total_row:
            moves.append(((0, col),))
        else:
            moves.append(((next_row, col),))

        # Left
        next_col = col - 1
        if next_col < 0:
            moves.append(((row, total_col - 1),))
        else:
            moves.append(((row, next_col),))

        # Right
        next_col = col + 1
        if next_col >= total_col:
            moves.append(((row, 0),))
        else:
            moves.append(((row, next_col),))

        return moves

    def _find_closest_ressource(self, ressource: str) -> List:
        """Return the path to the closest resource."""
        rows = len(self.map)
        cols = len(self.map[0]) if rows > 0 else 0

        visited = set()
        queue = deque()
        
        start = tuple(self.position)
        queue.append((start, [start]))
        visited.add(start)
        
        while queue:
            (row, col), path = queue.popleft()
            tile_contents = self.map[row][col].split()
            
            if ressource in tile_contents:
                return path
            
            for move in self._get_neighbors(row, col, rows, cols):
                if len(move) == 1:
                    (nr, nc) = move[0]
                    if (nr, nc) not in visited:
                        visited.add((nr, nc))
                        queue.append(((nr, nc), path + [(nr, nc)]))
                else:
                    (virtual, wrapped) = move
                    if wrapped not in visited:
                        visited.add(wrapped)
                        queue.append((wrapped, path + [virtual, wrapped]))
        
        return None

    def _path_to_tile(self, target_y: int, target_x: int) -> List:
        """Return the path to the given coordinates."""
        rows = len(self.map)
        cols = len(self.map[0]) if rows > 0 else 0

        visited = set()
        queue = deque()

        start = tuple(self.position)
        queue.append((start, [start]))
        visited.add(start)

        while queue:
            (row, col), path = queue.popleft()

            if (row, col) == (target_y, target_x):
                return path

            for move in self._get_neighbors(row, col, rows, cols):
                if len(move) == 1:
                    (nr, nc) = move[0]
                    if (nr, nc) not in visited:
                        visited.add((nr, nc))
                        queue.append(((nr, nc), path + [(nr, nc)]))
                else:
                    (virtual, wrapped) = move
                    if wrapped not in visited:
                        visited.add(wrapped)
                        queue.append((wrapped, path + [virtual, wrapped]))

        return None

    def parse_vision(self, view: str):
        """Actualize map base on the look command"""
        view = [v.strip() for v in view.strip("[]").split(",")]
        width = len(self.map[0])
        height = len(self.map)

        # Forward direction vectors (step along depth)
        DIRS = {
            'N': (-1, 0),
            'E': (0, 1),
            'S': (1, 0),
            'W': (0, -1)
        }

        # Right vector (used for lateral sweep at each depth)
        LEFT = {
            'N': (0, -1),   # player facing North: right is +x
            'E': (-1, 0),   # player facing East:  right is +y
            'S': (0, 1),  # player facing South: right is -x
            'W': (1, 0)   # player facing West:  right is -y
        }

        dir_vec = DIRS[self.orientation]
        left_vec = LEFT[self.orientation]

        i = 0
        depth = 0

        while i < len(view):
            # Move forward by 'depth' steps
            center_x = (self.position[1] + dir_vec[1] * depth) % width
            center_y = (self.position[0] + dir_vec[0] * depth) % height

            # Sweep laterally at this depth, right to left
            for j in range(depth, -depth - 1, -1):
                dx = (center_x + left_vec[1] * j) % width
                dy = (center_y + left_vec[0] * j) % height

                if i >= len(view):
                    break

                objects = [o for o in view[i].strip().split() if o != "player"]
                if objects:
                    self.map[dy][dx] = ' '.join(objects)
                else:
                    self.map[dy][dx] = ""
                i += 1
            depth += 1
    
    def parse_inventory(self, inv: str):
        """Parse inventory response"""
        inv = inv.replace('[', '').replace(']', '').replace('\n', '').strip()
        items = [item.strip() for item in inv.split(',') if item.strip()]
        
        for item in items:
            parts = item.strip().split()
            try:
                if len(parts) >= 2:
                    resource_name = parts[0]
                    quantity = int(parts[1])
                    
                    if resource_name == "food":
                        self.food = quantity
                    elif resource_name in self.inventory.keys():
                        self.inventory[resource_name] = quantity
            except (ValueError, IndexError) as e:
                continue
                
        print(f"INVENTORY: food: {self.food}, linemate: {self.inventory['linemate']}, deraumere: {self.inventory['deraumere']}, sibur: {self.inventory['sibur']}, mendiane: {self.inventory['mendiane']}, phiras: {self.inventory['phiras']}, thystame: {self.inventory['thystame']}")
    
    def recalculate_position(self, position):
        if position[1] >= self.map_width:
            position[1] = 0
        if position[1] < 0:
            position[1] = self.map_width - 1
        
        if position[0] >= self.map_height:
            position[0] = 0
        if position[0] < 0:
            position[0] = self.map_height - 1

    def move_forward(self):
        """Change player position"""
        if self.orientation == "N":
            self.position[0] -= 1
        elif self.orientation == "S":
            self.position[0] += 1
        elif self.orientation == "E":
            self.position[1] += 1
        elif self.orientation == "W":
            self.position[1] -= 1
        
        self.recalculate_position(self.position)
        
        print(f"New position: {self.position[0]} {self.position[1]}")
    
    def turn_left(self):
        """Change player orientation"""
        if self.orientation == "N":
            self.orientation = "W"
        elif self.orientation == "S":
            self.orientation = "E"
        elif self.orientation == "E":
            self.orientation = "N"
        elif self.orientation == "W":
            self.orientation = "S"
        print(f"New orientation: {self.orientation}")
    
    def turn_right(self):
        """Change player orientation"""
        if self.orientation == "N":
            self.orientation = "E"
        elif self.orientation == "S":
            self.orientation = "W"
        elif self.orientation == "E":
            self.orientation = "S"
        elif self.orientation == "W":
            self.orientation = "N"
        print(f"New orientation: {self.orientation}")
    
    def take_object(self, object: str):
        """Update player inventory"""
        object = object.strip()
        if object == "food":
            self.food += 1
        elif object in self.inventory.keys():
            self.inventory[object] += 1
        else:
            return
        self.map[self.position[0]][self.position[1]] = self.map[self.position[0]][self.position[1]].replace(object, '', 1).strip()

    def _print_map(self):
        """Print map"""
        print(f"Player pos : Y: {self.position[0]} X: {self.position[1]} {self.orientation}")
        for y, line in enumerate(self.map):
            for x, tile in enumerate(line):
                if tile == "":
                    tile = "_"
                if ([y, x] == self.position):
                    print("player " + tile, end='|')
                else:
                    print(tile, end='|')
            print("")

    def eject(self, direction: str):
        """Eject command"""
        DIRECTION = {
            "N": "S",
            "S": "N",
            "E": "W",
            "W": "E",
        }
        if direction not in DIRECTION:
            print(f"Ejection: invalid direction: {direction}")
            return
        self.orientation = DIRECTION[direction]
        print(f"Ejected. New orientation: {self.orientation}")

    def handle_incantation_broadcast(self, direction: str):
        """When receiving an incantation broadcast, goes to given direction"""
        self.has_received_incantation_broadcast = True
        direction = direction.strip()
        if direction == "0":
            # Send only once IAmHere
            if not self.has_arrived_to_incantation:
                self.zappy.client.clear_command_queue()
                self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-IAmHere_" + str(self.player_id))
            print("(RECEIVED DIRECTION 0)")
            self.has_arrived_to_incantation = True
            self.has_said_not_here = False
            return

        if not self.has_said_not_here:
            print("[handle_incantation_broadcast]")
            self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-IAmNotHere_" + str(self.player_id))
            self.has_said_not_here = True

        self.has_arrived_to_incantation = False

        if self.food < 5:
            self.look_for_food()
            return

        last_cmds = self.zappy.client.last_cmds
        client = self.zappy.client

        if self.zappy.client.last_cmds.qsize() != 0 and client.moving_in_queue(last_cmds):
            print("[handle_incantation_broadcast]: MOVING COMMAND ALREADY IN QUEUE")
            return

        DIRECTIONS = [ "1", "2", "3", "4", "5", "6", "7", "8" ]

        if direction not in DIRECTIONS:
            print("Broadcast: Invalid direction:", direction)
            return

        pos = self.get_relative_tile(int(direction))
        self.recalculate_position(pos)
        print(f"DEBUG: orientation = {self.orientation} | position = {self.position} | direction = {direction} | tile = {pos}")

        path = self._path_to_tile(pos[0], pos[1])

        if not path:
            print(f"Broadcast: Invalid path for direction: {direction}")
            return

        self.wanted_incantation = False

        if len(path) == 1:
            self.has_incantation_objective = False
            self.incantation_objective_path = []
            return

        self.has_incantation_objective = True
        self.incantation_objective_path = path

        if self.zappy.client.last_cmds.qsize() + len(path) >= 10:
            return

        self._determine_next_action(path, None)
    
    def handle_start_incantation(self, command: str, direction: str):
        """Handle command StartIncantation and EndOfIncantation"""
        if command.startswith("StartIncantation"):
            self.can_move = False
        elif command.startswith("EndOfIncantation"):
            self.can_move = True
            self.has_incantation_objective = False
            self.has_arrived_to_incantation = False
            self.has_received_incantation_broadcast = False
            self.zappy.client.send_command("Inventory")

    def update_character_counter(self, command: str):
        """Update counter for number of characters"""
        if command.startswith("NewCharacter"):
            parts = command.split('_')
            if len(parts) < 4:
                return

            player_id = int(parts[1])
            self.players_info[player_id] = int(parts[3])
            self.tot_player_here[player_id] = False
            print("[+] New CHARACTER:", int(parts[1]), "level:", int(parts[3]))

            self.character_counter += 1
            self.zappy.client.send_command("Broadcast " + self.zappy.team_name + "-Total_character:" + str(self.character_counter))
        elif command.startswith("Total_character"):
            parts = command.split(':')

            if len(parts) <= 1:
                return
            try:
                tot = int(parts[1])
                if tot > self.character_counter:
                    self.character_counter = tot
                    print("New total of characters:", self.character_counter)
            except ValueError:
                return
        elif command.startswith("Death"):
            parts = command.split('_')
            if len(parts) < 2:
                return
            
            player_id = int(parts[1])
            print("Receive death for player:", player_id)
            if player_id in self.players_info.keys():
                self.tot_all_player -= 1
                self.wanted_incantation = False
                self.character_counter -= 1
                self.players_info.pop(player_id)
                print("Player", int(parts[1]), "died. New total of player:", self.character_counter)
            else:
                print("Not removed. Dict:", self.players_info)
            
            if player_id in self.tot_player_here.keys():
                self.tot_player_here.pop(player_id)

    def has_resources_for_level(self, target_level: int) -> bool:
        """Check if we have all resources needed for a specific level incantation"""
        if target_level not in INCANTATION_REQUIREMENTS:
            return False
        
        requirements = INCANTATION_REQUIREMENTS[target_level]['resources']
        for resource, needed in requirements.items():
            if self.inventory.get(resource, 0) < needed:
                return False
        return True

    def get_missing_resources_for_level(self, target_level: int) -> Dict[str, int]:
        """Get a dict of missing resources for a specific level"""
        missing = {}
        if target_level not in INCANTATION_REQUIREMENTS:
            return missing
            
        requirements = INCANTATION_REQUIREMENTS[target_level]['resources']
        for resource, needed in requirements.items():
            current = self.inventory.get(resource, 0)
            if current < needed:
                missing[resource] = needed - current
        return missing
    
    def handle_level_information(self, level_str: str, player_id_str: str):
        """Fill the information about the other player"""
        level = level_str.split('_')
        if len(level) < 2:
            return
        level_nb = int(level[1])

        player = player_id_str.split('_')
        if len(player) < 2:
            return
        player_id = int(player[1])

        print("[+] LEVEL UP - player:", player_id, "level:", level_nb)
        self.players_info[player_id] = level_nb
        if player_id not in self.tot_player_here.keys():
            self.tot_player_here[player_id] = False
    
    def all_player_are_here(self) -> bool:
        for here in self.tot_player_here.values():
            if not here:
                return False
        return True
    
    def reset_player_here(self):
        """Reset player_here dict"""
        self.tot_player_here = {key: False for key in self.tot_player_here}

    def player_is_here(self, message: str, here: bool):
        """Handle IAmHere message"""
        parts = message.split('_')
        if len(parts) < 2:
            return
        player_id = int(parts[1])
        if player_id in self.tot_player_here.keys():
            self.tot_player_here[player_id] = here
    
    def change_first_process(self, content: str):
        """Handle broadcast : change of first process"""
        parts = content.split('_')
        if len(parts) > 1:
            id = int(parts[1])
            if self.player_id == id:
                self.fork_manager.is_first_process = True


def direction_to(from_y: int, from_x: int, to_y: int, to_x: int, max_y: int, max_x: int) -> str | None:
    """
    Determine the shortest cardinal direction to move from (y1, x1) to (y2, x2)
    on a map that wraps around (toroidal).

    Returns:
        One of: 'N', 'S', 'E', 'W'
    """

    # Compute shortest distance in x direction (East/West)
    dx = (to_x - from_x) % max_x
    if dx > max_x // 2:
        dx -= max_x  # Go West instead of long East

    # Compute shortest distance in y direction (North/South)
    dy = (to_y - from_y) % max_y
    if dy > max_y // 2:
        dy -= max_y  # Go North instead of long South

    # Prefer vertical movement first
    if abs(dy) >= abs(dx):
        if dy < 0:
            return 'N'
        elif dy > 0:
            return 'S'
    if dx < 0:
        return 'W'
    elif dx > 0:
        return 'E'

    return None