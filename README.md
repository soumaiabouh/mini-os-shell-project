# Mini OS Shell Project

University course project where we built a mini OS shell in C. The shell includes basic command functionalities such as 'quit', 'set', 'print', 'run', 'echo', 'ls', 'mkdir', 'touch', and 'cd'. We also implemented a simple CPU scheduler for process management using four different scheduling algorithms - First-Come-First-Serve (FCFS), Shortest-Job-First (SJF), Round-Robin (RR) and Aging. The scheduler was implemented using a priority queue data structure and the system uses PCBs (Process Control Blocks) to keep track of each process.

**Collaborators:** Soumaia Bouhouia and Alex Andrianavalontsalama

**Skills Demonstrated:** C programming, process management, priority queue data structure, CPU scheduling algorithms.

## Project Overview

This project demonstrates our ability to design and implement a functional mini OS shell in C, and our knowledge of process management and CPU scheduling algorithms. It showcases our skills in programming, data structures, and algorithm design, and provides an opportunity for us to apply the knowledge we gained in the university course.

## Project Structure

```
mini-os-shell-project/
├── src/                    # Source code directory
│   ├── shell.c              # Main shell implementation
│   ├── shell.h              # Shell header file
│   ├── interpreter.c        # Command interpreter
│   ├── interpreter.h        # Interpreter header file
│   ├── shellmemory.c        # Memory management for variables
│   ├── shellmemory.h        # Memory management header
│   ├── policies.c           # CPU scheduling policies (FCFS, SJF, RR, Aging)
│   ├── policies.h           # Policies header file
│   ├── helper.c             # Utility helper functions
│   ├── helper.h             # Helper header file
│   ├── Makefile             # Build configuration
│   └── Autograder.bash      # Automated testing script
├── testcases/               # Test case directories
│   ├── assignment2/         # Test cases for assignment 2 (scheduling algorithms)
│   └── assignment3/         # Test cases for assignment 3 (advanced features)
├── .vscode/                 # VS Code configuration
├── devtool.sh              # Development tools script
├── .gitignore              # Git ignore file
└── README.md               # This file
```

## Key Components

### Shell Commands
- **quit**: Exit the shell
- **set**: Set shell variables
- **print**: Print variable values
- **run**: Execute programs with specified scheduling algorithm
- **echo**: Display messages
- **ls**: List directory contents
- **mkdir**: Create directories
- **touch**: Create files
- **cd**: Change directory

### CPU Scheduling Algorithms
1. **FCFS (First-Come-First-Serve)**: Processes execute in the order they arrive
2. **SJF (Shortest-Job-First)**: Process with shortest execution time runs first
3. **RR (Round-Robin)**: Each process gets a time quantum before being preempted
4. **Aging**: Priority-based scheduling that prevents starvation

### Data Structures
- **Priority Queue**: Used for efficient process scheduling
- **Process Control Blocks (PCBs)**: Track process metadata and state

## Installation & Setup

### Prerequisites
- **GCC compiler** (or any compatible C compiler)
- **Make** build tool
- **Linux/Unix environment** (or Windows with WSL, Cygwin, or Git Bash)

### Building the Shell

1. **Navigate to the source directory:**
   ```bash
   cd src
   ```

2. **Build the executable:**
   ```bash
   make mysh
   ```

   Or with custom parameters:
   ```bash
   make mysh framesize=1024 varmemsize=512
   ```

3. **Run the shell:**
   ```bash
   ./mysh
   ```

4. **Clean build artifacts:**
   ```bash
   make clean
   ```

### Compiler Flags

- `-g`: Include debug symbols for debugging
- `-D FRAME_STORE_SIZE`: Set the frame store size (memory allocation)
- `-D VAR_STORE_SIZE`: Set the variable store size (variable storage)

## Testing

The project includes automated test cases in the `testcases/` directory:

- **assignment2/**: Contains test cases for scheduling algorithms (FCFS, SJF, RR, Aging)
- **assignment3/**: Contains advanced test cases

To run the autograder:
```bash
bash Autograder.bash
```

## Usage Example

Once the shell is running, you can execute commands:

```bash
mysh> set myvar 10
mysh> print myvar
mysh> run program_name FCFS
mysh> quit
```

## Notes

- The shell supports internal variables and environment interaction
- Scheduling algorithms can be tested using the provided test cases
- Each test case includes expected output for validation
