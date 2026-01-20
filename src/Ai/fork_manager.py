import subprocess
import random
import fcntl
import os

MAX_FORK_FILE = "/tmp/max_fork_"
FORK_LOCK_FILE = "/tmp/fork_"
DEFAULT_MAX_FORK = 7


class ForkManager:
    def __init__(self, map_width: int, map_height: int, team_name: str, player_id: int):
        self.map_height = map_height
        self.map_width = map_width

        self.fork_in_progress = False

        self.team_name = team_name
        self.txt_name = MAX_FORK_FILE + self.team_name + ".txt"
        self.lock_name = FORK_LOCK_FILE + self.team_name + ".lock"

        self.player_id = player_id

        self.load_max_fork(True)

        
        self.fork_processes = []  # Track spawned processes
        self.successful_forks = 0

        print("[?] Is first process:", self.is_first_process)

    def load_max_fork(self, change_first_process: bool):
        try:
            with open(self.txt_name, "r") as f:
                self.max_fork = int(f.read())
            if change_first_process:
                self.is_first_process = False
        except FileNotFoundError:
            self.max_fork = DEFAULT_MAX_FORK
            self.save_max_fork()
            if change_first_process:
                self.is_first_process = True

    def save_max_fork(self):
        with open(self.txt_name, "w") as f:
            f.write(str(self.max_fork))

    def try_fork(self, zappy_port: int, team_name: str, auto_join: bool, character_counter: int):
        with open(self.lock_name, "w") as lock:
            try:
                fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
            except BlockingIOError:
                # Another process is already forking
                return

            self.load_max_fork(False)

            if character_counter >= 8 and self.max_fork <= 0:
                return

            self.max_fork -= 1
            self.save_max_fork()

            if auto_join:
                self._spawn_new_process(zappy_port, team_name)

    def can_fork(self, food: int, character_counter: int) -> bool:
        """Check if the condition to fork are met"""
        print("[CAN_FORK]: is_first_process:", self.is_first_process, "food:", food, "max_fork:", self.max_fork, "character_counter:", character_counter)
        if self.is_first_process and food >= 18 and character_counter < 8 and self.max_fork >= 1 and not self.fork_in_progress:
            print("[CAN FORK = TRUE]")
            self.forked = True
            return True
        return False

    def _spawn_new_process(self, zappy_port: int, team_name: str) -> bool:
        """Spawn a new AI process"""
        try:
            # Check if we can spawn more processes
            self._cleanup_dead_processes()
            
            os.makedirs("logs", exist_ok=True)
            
            log_id = f"{random.randint(1000, 9999)}"

            log_filename = f"logs/ai_{team_name}_{self.player_id}_{log_id}.log"
            
            with open(log_filename, 'w') as log_file:
                process = subprocess.Popen(
                    ["python3", "-m", "src.Ai.main", "-p", str(zappy_port), "-n", team_name],
                    stdout=log_file,
                    stderr=subprocess.STDOUT,
                    stdin=subprocess.DEVNULL,
                    close_fds=True,
                    start_new_session=True
                )
                
                self.fork_processes.append({
                    'process': process,
                    'pid': process.pid,
                    'log_file': log_filename,
                    'fork_number': self.successful_forks
                })
                
                print(f"[+] New AI process spawned successfully!")
                
                self.successful_forks += 1
                self._cleanup_dead_processes()
                
                return True
                
        except Exception as e:
            print(f"Failed to spawn new AI process: {e}")
            return False
    
    def _cleanup_dead_processes(self):
        """Remove terminated processes from tracking list"""
        alive_processes = []
        
        for fork_info in self.fork_processes:
            process = fork_info['process']
            # Check if process is still running
            if process.poll() is None:
                alive_processes.append(fork_info)
            else:
                print(f"Process {fork_info['pid']} (Fork #{fork_info['fork_number']}) has terminated")
        
        self.fork_processes = alive_processes
    
    def shutdown_all_forks(self):
        """Shutdown all spawned processes"""
        print(f"Shutting down {len(self.fork_processes)} forked processes")
        
        for fork_info in self.fork_processes:
            process = fork_info['process']
            if process.poll() is None:
                try:
                    process.terminate()
                    process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    process.kill()
                except Exception as e:
                    print(f"Error shutting down process {fork_info['pid']}: {e}")
        
        self.fork_processes.clear()