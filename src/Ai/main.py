#!/usr/bin/env python3

import argparse
import sys
from src.Ai.zappy import Zappy

def parse_arguments():
    """Parse command line arguments"""
    parser = argparse.ArgumentParser(description='Zappy AI Client')
    parser.add_argument('-p', '--port', type=int, required=True, 
                       help='Port number')
    parser.add_argument('-n', '--name', type=str, required=True, 
                       help='Team name')
    parser.add_argument('-m', '--machine', type=str, default='localhost', 
                       help='Server hostname (default: localhost)')
    parser.add_argument('-a', '--auto-join', action='store_true', 
                       help='Enable fork auto join')
    return parser.parse_args()

def main():
    """Main function"""
    args = parse_arguments()

    print(f"Connecting to {args.machine}:{args.port}")
    print(f"Team: {args.name}")

    ai = Zappy(args.machine, args.port, args.name, args.auto_join)
    
    if not ai.connect():
        print("Failed to connect to server")
        sys.exit(1)
    
    print(f"Connected successfully!")
    ai.run()

if __name__ == "__main__":
    main()