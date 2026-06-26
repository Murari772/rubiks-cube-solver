#include <iostream>
#include <queue>
#include <chrono>
#include "../include/RubiksCubeBitboard.h"
#include "../include/pattern_databases/CornerPatternDatabase.h"

int main() {
    std::cout << " Generating Corner Patterns\n";

    CornerPatternDatabase cornerDB;
    RubiksCubeBitboard solvedCube;
    cornerDB.setNumMoves(solvedCube, 0);

    std::queue<std::pair<RubiksCubeBitboard, uint8_t>> q;
    q.push({solvedCube, 0});

    auto start = std::chrono::high_resolution_clock::now();
    uint8_t currentDepth = 0;

    while (!q.empty()) {
        auto [cube, depth] = q.front();
        q.pop();

        if (depth > currentDepth) {
            currentDepth = depth;
            std::cout << "Reached Depth " << (int)currentDepth 
                      << " | States Discovered: " << cornerDB.getNumElements() 
                      << " / " << cornerDB.getSize() << "\n";
        }

        for (int i = 0; i < 18; i++) {
            RubiksCube::MOVE move = static_cast<RubiksCube::MOVE>(i);
            cube.move(move);
            
            if (cornerDB.setNumMoves(cube, depth + 1)) {
                q.push({cube, depth + 1});
            }
            cube.invert(move);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\nTime taken: " << duration.count() << " seconds.\n";
    
    if (cornerDB.saveDatabase("corner_db.bin")) {
        std::cout << " Successfully saved to: corner_db.bin\n";
    } else {
        std::cout << " ERROR: Failed to save to disk.\n";
    }

    return 0;
}