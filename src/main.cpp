#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "../include/RubiksCubeBitboard.h"
#include "../include/pattern_databases/CornerPatternDatabase.h"
#include "../include/pattern_databases/EdgePatternDatabase.h"
#include "../include/Solver.h"

std::vector<RubiksCube::MOVE> generateScramble(int numMoves) {
    std::vector<RubiksCube::MOVE> scramble;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> faceDist(0, 5); 
    std::uniform_int_distribution<> turnDist(0, 2); 
    
    int lastFace = -1;
    int secondLastFace = -1;
    
    auto areOpposite = [](int f1, int f2) {
        if (f1 == 0 && f2 == 5) return true;
        if (f1 == 5 && f2 == 0) return true;
        if (f1 == 1 && f2 == 3) return true;
        if (f1 == 3 && f2 == 1) return true;
        if (f1 == 2 && f2 == 4) return true;
        if (f1 == 4 && f2 == 2) return true;
        return false;
    };

    for (int i = 0; i < numMoves; ++i) {
        int currentFace = faceDist(gen);
        
        while (true) {
            bool valid = true;
            
            if (currentFace == lastFace) valid = false;
            
            if (lastFace != -1 && areOpposite(currentFace, lastFace) && currentFace < lastFace) {
                valid = false;
            }
            
            if (secondLastFace != -1 && currentFace == secondLastFace && areOpposite(currentFace, lastFace)) {
                valid = false;
            }

            if (valid) break;
            currentFace = faceDist(gen);
        }
        
        secondLastFace = lastFace;
        lastFace = currentFace;
        
        int turnType = turnDist(gen);
        scramble.push_back(static_cast<RubiksCube::MOVE>((currentFace * 3) + turnType));
    }
    return scramble;
}

int main(int argc, char** argv) {
    std::string algorithm = "idastar";
    int scrambleLength = 13;

    if (argc > 1) {
        algorithm = argv[1];
    }
    if (argc > 2) {
        scrambleLength = std::stoi(argv[2]);
    }

    std::cout << "Initializing Engine...\n";
    RubiksCubeBitboard cube;
    CornerPatternDatabase cornerDB;
    EdgePatternDatabase edgeDB1(0);
    EdgePatternDatabase edgeDB2(6);

    if (algorithm == "idastar") {
        if (!cornerDB.loadDatabase("corner_db.bin") ||
            !edgeDB1.loadDatabase("edge_db_1.bin") ||
            !edgeDB2.loadDatabase("edge_db_2.bin")) {
            std::cout << "CRITICAL ERROR: Could not load pattern databases. Did you run the generator?\n";
            return 1;
        }
    }
    
    std::vector<RubiksCube::MOVE> scrambleSequence = generateScramble(scrambleLength);
    std::cout << "\nScrambling with " << scrambleLength << " moves: ";
    for (auto move : scrambleSequence) {
        std::cout << RubiksCube::getMoveString(move) << " ";
        cube.move(move);
    }
    std::cout << "\n\n";

    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<RubiksCube::MOVE> solution;
    if (algorithm == "iddfs") {
        solution = Solver::solveIDDFS(cube, 20);
    } else {
        solution = Solver::solveIDAStar(cube, cornerDB, edgeDB1, edgeDB2, 20);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\n=== FINAL RESULTS ===\n";
    std::cout << "Algorithm used: " << algorithm << "\n";
    if (!solution.empty()) {
        std::cout << "Solution Path: ";
        for (auto move : solution) {
            std::cout << RubiksCube::getMoveString(move) << " ";
        }
        std::cout << "\nSolution Length: " << solution.size() << " moves\n";
    }
    std::cout << "Time Taken: " << duration.count() << " ms (" << duration.count() / 1000.0 << " seconds)\n";

    return 0;
}