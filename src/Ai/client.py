import socket
import threading
import queue
from typing import Optional

class NetworkClient:
    """TCP client for communicating with Zappy server"""
    
    def __init__(self, hostname: str, port: int, team_name: str, game: 'Game'):
        self.hostname = hostname
        self.port = port
        self.team_name = team_name
        self.socket: Optional[socket.socket] = None
        
        # Command queue (max 10 pending commands per protocol)
        self.command_queue = queue.Queue(maxsize=10)
        self.response_queue = queue.Queue()
        
        # Threading control
        self.running = False
        self.send_thread: Optional[threading.Thread] = None
        self.receive_thread: Optional[threading.Thread] = None
        
        # Connection info
        self.world_width = 0
        self.world_height = 0
        self.available_slots = 0
        self.last_cmds = queue.Queue(maxsize=10)

        self.game = game
        
    def connect(self) -> bool:
        """Connect to server and perform initial handshake"""
        try:
            # Create socket connection
            print(f"Connecting to {self.hostname}:{self.port}...")
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(10)  # 10 second timeout
            self.socket.connect((self.hostname, self.port))
            
            # Receive WELCOME
            welcome_msg = self._receive_line()
            print(f"Server: {welcome_msg}")
            
            if welcome_msg != "WELCOME":
                print(f"Expected WELCOME, got: {welcome_msg}")
                return False
            
            # Send team name
            print(f"Sending team name: {self.team_name}")
            self._send_line(self.team_name)

            # Receive client number (available slots)
            client_num_msg = self._receive_line()
            print("Server:", client_num_msg)
            
            # Receive world dimensions
            world_size_msg = self._receive_line()
            try:
                dimensions = world_size_msg.split()
                if len(dimensions) == 2:
                    self.world_width = int(dimensions[0])
                    self.world_height = int(dimensions[1])
                    print(f"World size: {self.world_width}x{self.world_height}")
                else:
                    print(f"Invalid world size format: {world_size_msg}")
                    return False
            except ValueError:
                print(f"Invalid world dimensions: {world_size_msg}")
                return False
            
            # Start communication threads
            self.running = True
            self.send_thread = threading.Thread(target=self._send_loop, daemon=True)
            self.receive_thread = threading.Thread(target=self._receive_loop, daemon=True)
            
            self.send_thread.start()
            self.receive_thread.start()
            
            print("Connection established successfully!")
            return True
            
        except socket.timeout:
            print("Connection timeout")
            return False
        except ConnectionRefusedError:
            print("Connection refused - is the server running?")
            return False
        except Exception as e:
            print(f"Connection error: {e}")
            return False
    
    def _receive_line(self) -> str:
        """Receive a single line from the server"""
        buffer = ""
        while True:
            char = self.socket.recv(1).decode('utf-8')
            if char == '\n':
                return buffer.strip()
            buffer += char
    
    def _send_line(self, message: str):
        """Send a single line to the server"""
        full_message = f"{message}\n"
        self.socket.send(full_message.encode('utf-8'))
    
    def send_command(self, command: str) -> bool:
        """Send command to server (non-blocking)"""
        try:
            print(f"-> Sent: {command}")

            if command == "Forward":
                self.game.move_forward()
            elif command == "Right":
                self.game.turn_right()
            elif command == "Left":
                self.game.turn_left()

            self.last_cmds.put_nowait(command)
            self.command_queue.put_nowait(command)
            return True
        except queue.Full:
            print(f"Command queue full! Dropping command: {command}")
            return False
    
    def get_response(self, timeout: float = 0.1) -> Optional[str]:
        """Get next response from server (non-blocking)"""
        try:
            return self.response_queue.get(timeout=timeout)
        except queue.Empty:
            return None
    
    def _send_loop(self):
        """Background thread for sending commands"""
        print("Send thread started")
        while self.running:
            try:
                command = self.command_queue.get(timeout=0.1)
                if command and self.socket:
                    self._send_line(command)
                    self.command_queue.task_done()
            except queue.Empty:
                continue
            except Exception as e:
                print(f"Send error: {e}")
                break
        print("Send thread stopped")
    
    def _receive_loop(self):
        """Background thread for receiving responses"""
        print("Receive thread started")
        buffer = ""
        
        while self.running:
            try:
                if not self.socket:
                    break
                
                # Receive data with timeout
                self.socket.settimeout(0.1)
                data = self.socket.recv(1024).decode('utf-8')
                
                if not data:
                    print("Server closed connection")
                    break
                
                buffer += data
                
                # Process complete lines
                while '\n' in buffer:
                    line, buffer = buffer.split('\n', 1)
                    line = line.strip()
                    if line:
                        self.response_queue.put(line)
                        
            except socket.timeout:
                continue
            except Exception as e:
                print(f"Receive error: {e}")
                break
        
        print("Receive thread stopped")
    
    def disconnect(self):
        """Clean shutdown of the connection"""
        print("Disconnecting...")
        self.running = False
        
        # Wait for threads to finish
        if self.send_thread and self.send_thread.is_alive():
            self.send_thread.join(timeout=1)
        if self.receive_thread and self.receive_thread.is_alive():
            self.receive_thread.join(timeout=1)
        
        # Close socket
        if self.socket:
            try:
                self.socket.close()
            except:
                pass
            self.socket = None
        
        print("Disconnected")
    
    def clear_command_queue(self):
        """Clear both command queue and last_cmd"""
        try:
            while self.command_queue:
                self.command_queue.get_nowait()
        except queue.Empty:
            pass
        try:
            while self.last_cmds:
                self.last_cmds.get_nowait()
        except queue.Empty:
            pass

    def moving_in_queue(self, q):
        temp_queue = queue.Queue()
        found = False

        while not q.empty():
            item = q.get()
            if item == "Right" or item == "Left" or item == "Forward":
                found = True
            temp_queue.put(item)

        # Restore the original queue
        while not temp_queue.empty():
            q.put(temp_queue.get())

        return found