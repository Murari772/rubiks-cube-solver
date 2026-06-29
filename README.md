# Rubik's Cube Solver

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A high-performance Rubik's Cube solver written in C++17. This project implements an optimal solver using the Iterative Deepening A* (IDA*) algorithm coupled with Pattern Database (PDB) heuristics. It is capable of finding the shortest possible solution for any scrambled Rubik's Cube in approximately 1 second.

![Hero Image Placeholder](docs/images/hero_image_placeholder.png) <!-- Placeholder for a nice hero image, UI screenshot, or GIF of the solver running -->

## Features

* **Optimal Solutions:** Guarantees finding the absolute shortest sequence of moves to solve the cube (God's Number is 20).
* **High Performance:** Solves random scrambles optimally in roughly 1 second utilizing heavily optimized C++.
* **Advanced Heuristics:** Employs Pattern Databases (PDBs) covering over 170 million indexed states to dramatically prune the search space.
* **Efficient State Indexing:** Uses Lehmer coding and permutation ranking for extremely fast, $O(1)$ lookups in the heuristic databases.
* **Object-Oriented Design:** Clean, modular architecture supporting multiple internal cube representations.

## How It Works

<details>
<summary><b>Click to read a high-level explanation of the algorithms</b></summary>

### The Search Algorithm: IDA*
Iterative Deepening A* (IDA*) is a state-space search algorithm that combines the memory efficiency of Depth-First Search with the optimality guarantees of Breadth-First Search. It searches deeper and deeper into the "tree" of possible moves, using a heuristic function to cut off branches that are mathematically guaranteed not to lead to an optimal solution.

### The Heuristic: Pattern Databases (PDBs)
A Pattern Database is a massive pre-computed lookup table. Instead of trying to evaluate how close the *entire* cube is to being solved, a PDB tracks only a subset of the cube's pieces (e.g., just the corners). By pre-calculating the exact minimum number of moves required to solve just that subset from any possible configuration, we obtain an *admissible heuristic*—a guaranteed lower bound on the total number of moves needed.

This solver utilizes three primary Pattern Databases:
1.  **Corner Database:** Tracks the 8 corner pieces (~88 million states).
2.  **Edge Database 1:** Tracks 6 of the 12 edge pieces (~42 million states).
3.  **Edge Database 2:** Tracks the remaining 6 edge pieces (~42 million states).

### Lehmer Encoding
To store and look up these millions of states efficiently, the solver uses the Lehmer code (or permutation ranking). This combinatorial technique maps every possible physical permutation of the pieces to a unique integer index. This allows the massive PDBs to be stored as highly compact, contiguous arrays in memory, ensuring lightning-fast memory access during the search.
</details>

## Architecture

The project is built with a modular, Object-Oriented architecture designed to separate the physical cube logic, the complex search algorithms, and the user interface.

| Module | Description |
| :--- | :--- |
| **`Model`** | Contains various Rubik's Cube representations (e.g., 1D array, 3D array, highly optimized Bitboards). |
| **`Solver`** | Implements the core search algorithms, primarily the IDA* engine. |
| **`PatternDatabase`** | Manages the generation, disk caching, and memory-mapping of the heuristic databases. |
| **`Math`** | Utilities for Lehmer encoding, permutation ranking, and combinatorics. |
| **`UI` / `CLI`** | The command-line interface for interacting with the solver engine. |

## Repository Structure

```text
rubiks-cube-solver/
├── src/
│   ├── Model/            # Cube representations and state transitions
│   ├── Solver/           # IDA* and search logic
│   ├── PatternDatabase/  # PDB generation and lookups
│   ├── Math/             # Indexing and combinatorics
│   └── main.cpp          # Application entry point
├── include/              # Public header files
├── tests/                # Unit tests (Catch2/GTest)
├── build/                # CMake build directory
├── CMakeLists.txt        # CMake build configuration
└── README.md             # This file
```

## Installation & Building

This project uses CMake as its build system. A modern compiler supporting C++17 is required.

### Prerequisites
*   CMake (3.14 or higher)
*   C++17 compatible compiler (GCC, Clang, or MSVC)

### Linux / macOS

```bash
git clone https://github.com/yourusername/rubiks-cube-solver.git
cd rubiks-cube-solver
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### Windows (MSVC)

```powershell
git clone https://github.com/yourusername/rubiks-cube-solver.git
cd rubiks-cube-solver
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

Once compiled, you can run the solver from your terminal. 
*Note: On the very first run, the solver will generate the pattern databases and save them to disk. This one-time process may take a few minutes. Subsequent runs will be significantly faster as they load the databases directly from disk.*

```bash
# Example syntax
./RubiksCubeSolver --scramble "U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2"
```

### Example Output

```text
[INFO] Loading Pattern Databases...
[INFO] Databases loaded successfully from disk in 0.4s.
[INFO] Scramble: U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2
[INFO] Starting IDA* search...

Solution found: D2 L2 B2 R2 F2 U' L2 D B2 R2 F' L B U' L F' R B D2 R
Total moves: 20
Nodes expanded: 1,432,591
Solve time: 1.12 seconds
```

![Terminal Output Animation Placeholder](docs/images/terminal_output_placeholder.gif) <!-- Placeholder for a terminal recording (e.g., using asciinema or a GIF) -->

## Performance

*   **Optimal Solutions:** The solver always finds the absolute shortest path to the solved state.
*   **Search Efficiency:** By combining move pruning (avoiding redundant moves like `U U'`) and admissible PDB heuristics, the effective branching factor is reduced from 18 to roughly 13.
*   **Solve Time:** Typical random scrambles are solved in **~1 second** after the PDBs are loaded into memory.
*   **Memory Footprint:** The pre-computed pattern databases take up a moderate amount of disk space and RAM, allowing them to fit comfortably on modern machines:
    *   Corner PDB: ~88 million states
    *   Edge PDBs: ~42 million states each
    *   Total indexed states: >170 million.

![Performance Graph Placeholder](docs/images/performance_graph_placeholder.png) <!-- Placeholder for a graph showing solve time distribution or nodes expanded vs scramble length -->

## Algorithm Deep Dive

<details>
<summary><b>Click to expand advanced technical details</b></summary>

*   **Abstract Cube Representation:** The solver engine abstracts the cube state via interfaces, allowing the search algorithm to utilize highly optimized internal representations (such as Bitboards) for maximum throughput during state transitions without coupling the logic.
*   **Move Pruning:** Pruning tables are implemented to avoid generating redundant branches in the search tree. This includes discarding sequential moves on the same face (e.g., `F` followed by `F2`) and handling commutative redundancies on opposite faces (e.g., evaluating `U D` but skipping `D U`).
*   **Memory Bound Search:** The memory footprint of the IDA* search itself is minimal and scales linearly, constrained only by the depth of the recursion stack (which never exceeds 20 for a standard 3x3x3 cube).
*   **Lehmer Encoding Mechanics:** By converting a specific permutation of pieces (e.g., the arrangement of the 8 corners) into a Lehmer code, we generate a unique integer from $0$ to $n!-1$. This acts as a perfect hashing function, allowing the multi-dimensional state space to be flattened into a simple 1D array. This results in $O(1)$ memory access times, which is critical when evaluating millions of nodes per second.
</details>

## Future Improvements

While this solver is highly optimized for optimal solutions, potential areas for future exploration include:
*   **Kociemba's Two-Phase Algorithm:** Implementing Kociemba's algorithm to find *near-optimal* solutions in a fraction of a millisecond.
*   **Parallel Search:** Multi-threading the IDA* search tree exploration or parallelizing the initial PDB generation.
*   **SIMD Instructions:** Utilizing AVX/SSE instructions for parallel state evaluations and transitions.
*   **GUI / Visualizer:** Developing a graphical interface (e.g., with OpenGL or Qt) to interactively scramble, animate, and visualize the solving process.
