import re
import queue
from src.Ai.client import NetworkClient
from src.Ai.game import Game

class Zappy:
    """Main AI Controller - coordinates all components"""
    
    def __init__(self, hostname: str, port: int, team_name: str, auto_join: bool):
        self.hostname = hostname
        self.port = port
        self.team_name = team_name
        self.client = None
        self.running = False
        self.game = None
        self.auto_join = auto_join
        
    def connect(self) -> bool:
        """Connect to the Zappy server"""
        try:
            self.client = NetworkClient(self.hostname, self.port, self.team_name, None)
            if self.client.connect():
                self.game = Game(self.client.world_width, self.client.world_height, self, self.auto_join)
                self.client.game = self.game
                return True
            return False
        except Exception as e:
            print(f"Connection failed: {e}")
            return False
    
    def run(self):
        """Main AI execution loop"""
        print(f"Starting AI for team '{self.team_name}'...")
        self.running = True
        
        # Try to get a response
        response = self.client.get_response(timeout=0.1)
        if response:
            self.handle_response(response)

        # Initial setup
        self.client.send_command("Inventory")
        self.client.send_command("Broadcast " + self.team_name + "-NewCharacter_" + str(self.game.player_id) + "_Level_" + str(self.game.level))
        
        while self.running:
            try:
                # Process server response
                response = self.client.get_response(timeout=0.1)
                if response:
                    self.handle_response(response)
                
                if not self.running:
                    break

                # Make decisions at regular intervals (but not during incantation)
                if not self.game.is_incanting and self.client.last_cmds.qsize() < 1:
                    if self.game:
                        self.game.make_decision()

                        # Periodic inventory checks
                        if self.game.decision_counter % 5 == 1:
                            self.client.send_command("Inventory")
                
            except KeyboardInterrupt:
                print("\nShutting down AI...")
                self.running = False
                if self.game and self.game.fork_manager:
                    self.game.fork_manager.shutdown_all_forks()
        
        if self.client:
            self.client.disconnect()
    
    def handle_response(self, response: str):
        """Handle server responses"""
        print(f"=> Received : {response}")

        last_cmd = ""
        if self.client.last_cmds.qsize() > 0 and not response.startswith("Current level:") and not response.startswith("message"):
            try:
                last_cmd = self.client.last_cmds.get_nowait()
            except queue.Empty:
                last_cmd = ""

        if (last_cmd == "" and not response.startswith("Current level:") and not response.startswith("message")):
            print("No last command.")

        if (response.startswith("Eject ")):
            self.eject(response.split(' ')[1])
        elif response == "ok":
            self._ok_response(last_cmd)
        elif response == "ko":
            self._ko_response(last_cmd)
        elif response == "dead":
            print("Player died from starvation!")
            self.running = False
        elif self.is_inventory_response(response):
            self.game.parse_inventory(response)
        elif self.is_look_response(response):
            self.game.parse_vision(response)
        elif response.startswith("message"):
            self.parse_broadcast(response)
        elif response.startswith("Current level:"):
            print("Incantation ended.")
            splited = response.split(':')
            if len(splited) > 1:
                self.game.level = int(splited[1].strip())
                if self.game.fork_manager.is_first_process:
                    self.client.send_command("Broadcast " + self.team_name + "-EndOfIncantation")
                    self.game.reset_already_droped()
                self.client.send_command("Broadcast " + self.team_name + "-Level_" + splited[1].strip() + "-Player_" + str(self.game.player_id))
            self.game.is_incanting = False
            self.game.wanted_incantation = False
            self.game.has_arrived_to_incantation = False
            self.game.reset_player_here()
        elif response == "Elevation underway":
            self.game.wanted_incantation = False
            self.game.is_incanting = True
            self.game.has_arrived_to_incantation = False
        else:
            print(f"Unknown response: {response}")

    def _ko_response(self, last_cmd: str):
        print("Last command failed:", last_cmd)
        if last_cmd == "Incantation":
            if self.game.fork_manager.is_first_process:
                self.client.send_command("Broadcast " + self.team_name + "-EndOfIncantation")
                self.game.reset_already_droped()
            self.game.is_incanting = False
            self.client.send_command("Broadcast " + self.team_name + "-WantPlayer")
        elif last_cmd.startswith("Take"):
            parts = last_cmd.split(' ')
            if len(parts) > 1:
                if parts[1].strip() == "food":
                    self.game.food -= 1
                elif parts[1].strip() in self.game.inventory:
                    self.game.inventory[parts[1]] -= 1

    def _ok_response(self, last_cmd: str):
        print(f"Command succeeded: {last_cmd}")
        if last_cmd == "Fork":
            self.game.fork_manager.try_fork(self.port, self.team_name, self.auto_join, self.game.character_counter)
            self.game.fork_manager.fork_in_progress = False

    def is_inventory_response(self, response: str) -> bool:
        """Check if response is inventory data"""
        return (response.startswith('[') and 
                "food" in response and 
                re.search(r'food\s+\d+', response) is not None)
    
    def is_look_response(self, response: str) -> bool:
        """Check if response is vision data"""
        return (response.startswith('[') and "player" in response)
    
    def parse_broadcast(self, str: str):
        """Parse broadcast messages """
        message = str.split(',')
        print(f"Broadcast received: {str}")

        if len(message) <= 1:
            return

        direction = message[0].split(' ')
        message = message[1].split('-')
        if len(message) <= 1 or len(direction) <= 1: # Ignore other team messages
            self.client.send_command("Eject")
            return

        direction = direction[1].strip()

        if message[0].strip() != self.team_name: # Ignore other team messages
            if direction == "0":
                self.client.send_command("Eject")
            return

        content = message[1].strip()
        if content.startswith("WantPlayer"):
            self.game.handle_incantation_broadcast(direction)
        elif content.startswith("NewCharacter") or content.startswith("Total_character") \
            or content.startswith("Death"):
            self.game.update_character_counter(content)
        elif content.startswith("Level"):
            if len(message) >= 3:
                self.game.handle_level_information(content, message[2])
        elif content.startswith("IAmHere"):
            if self.game.fork_manager.is_first_process:
                self.game.player_is_here(content, True)
        elif content.startswith("IAmNotHere"):
            if self.game.fork_manager.is_first_process:
                self.game.player_is_here(content, False)
        elif content.startswith("StartIncantation") or content.startswith("EndOfIncantation"):
            self.game.handle_start_incantation(content, direction)
        elif content.startswith("FirstProcess"):
            self.game.change_first_process(content)

    def parse_eject(self, str: str):
        pass

def is_integer(s: str):
    return s.isdigit() or (s.startswith('-') and s[1:].isdigit())