#include <iostream>
#include <vector>
#include <random>
#include <chrono> 
#include "../include/RubiksCube3dArray.h"
#include "../include/Solver.h"
#include "../include/RubiksCubeBitboard.h"

std::vector<RubiksCube::MOVE> generateScramble(int numMoves) {
    std::vector<RubiksCube::MOVE> scramble;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> faceDist(0, 5); 
    std::uniform_int_distribution<> turnDist(0, 2); 

    auto areOpposite = [](int face1, int face2) {
        if (face1 == 0 && face2 == 5) return true;
        if (face1 == 5 && face2 == 0) return true;
        if (face1 == 1 && face2 == 3) return true;
        if (face1 == 3 && face2 == 1) return true;
        if (face1 == 2 && face2 == 4) return true;
        if (face1 == 4 && face2 == 2) return true;
        return false;
    };

    while (scramble.size() < numMoves) {
        int currentFace = faceDist(gen);

        if (!scramble.empty()) {
            int lastFace = static_cast<int>(scramble.back()) / 3;
            
            // Rule 1: Same Face
            if (currentFace == lastFace) continue;
            
            // Rule 2: Opposite Order
            if (areOpposite(currentFace, lastFace) && currentFace < lastFace) continue;
            
            // Rule 3: Sandwich Pruning
            if (scramble.size() >= 2) {
                int secondLastFace = static_cast<int>(scramble[scramble.size() - 2]) / 3;
                if (currentFace == secondLastFace && areOpposite(currentFace, lastFace)) continue;
            }
        }
        
        int turnType = turnDist(gen);
        int moveValue = (currentFace * 3) + turnType;
        scramble.push_back(static_cast<RubiksCube::MOVE>(moveValue));
    }
    return scramble;
}

int main() {
    //RubiksCube3dArray cube;
    RubiksCubeBitboard cube;
    
    int scrambleLength = 8; 
    int maxSearchDepth = 10; 
    
    std::vector<RubiksCube::MOVE> scrambleSequence = generateScramble(scrambleLength);
    
    std::cout << "Scrambling with " << scrambleLength << " moves: ";
    for (auto move : scrambleSequence) {
        std::cout << RubiksCube::getMoveString(move) << " ";
        cube.move(move);
    }
    std::cout << "\n\nStarting IDDFS Solver...\n";

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<RubiksCube::MOVE> solution = Solver::solveIDDFS(cube, maxSearchDepth);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\n=== RESULTS ===\n";
    if (!solution.empty()) {
        std::cout << "Solution Path: ";

        for (auto move : solution) {
            std::cout << RubiksCube::getMoveString(move) << " ";
        }

        std::cout << "\nSolution Length: " << solution.size() << " moves\n";
    } 
    else {
        std::cout << "Failed to find a solution within " << maxSearchDepth << " moves.\n";
    }
    
    std::cout << "Time Taken: " << duration.count() << " ms (" << duration.count() / 1000.0 << " seconds)\n";
    std::cout << "===============\n";

    return 0;
}