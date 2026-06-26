#include <iostream>
#include <queue>
#include <chrono>
#include <string>
#include "../include/RubiksCubeBitboard.h"
#include "../include/pattern_databases/CornerPatternDatabase.h"
#include "../include/pattern_databases/EdgePatternDatabase.h"

void generateCornerDB() {
    std::cout << "\n=== Generating Corner Patterns ===\n";

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
}

void generateEdgeDB(int startIndex, const std::string& fileName) {
    std::cout << "\n=== Generating Edge Patterns (Edges " << startIndex << " to " << (startIndex + 5) << ") ===\n";

    EdgePatternDatabase edgeDB(startIndex);
    RubiksCubeBitboard solvedCube;
    edgeDB.setNumMoves(solvedCube, 0);

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
                      << " | States Discovered: " << edgeDB.getNumElements() 
                      << " / " << edgeDB.getSize() << "\n";
        }

        for (int i = 0; i < 18; i++) {
            RubiksCube::MOVE move = static_cast<RubiksCube::MOVE>(i);
            cube.move(move);
            
            if (edgeDB.setNumMoves(cube, depth + 1)) {
                q.push({cube, depth + 1});
            }
            cube.invert(move);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "\nTime taken: " << duration.count() << " seconds.\n";
    
    if (edgeDB.saveDatabase(fileName)) {
        std::cout << " Successfully saved to: " << fileName << "\n";
    } else {
        std::cout << " ERROR: Failed to save to disk.\n";
    }
}

int main(int argc, char** argv) {
    if (argc > 1) {
        std::string target = argv[1];
        if (target == "corner") {
            generateCornerDB();
        } else if (target == "edge1") {
            generateEdgeDB(0, "edge_db_1.bin");
        } else if (target == "edge2") {
            generateEdgeDB(6, "edge_db_2.bin");
        } else {
            std::cout << "Unknown target: " << target << "\n";
            std::cout << "Usage: ./DatabaseGenerator [corner|edge1|edge2]\n";
        }
    } else {
        std::cout << "No target specified. Generating ALL databases sequentially...\n";
        generateCornerDB();
        generateEdgeDB(0, "edge_db_1.bin");
        generateEdgeDB(6, "edge_db_2.bin");
    }
    
    return 0;
}