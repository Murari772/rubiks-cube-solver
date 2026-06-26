#ifndef SOLVER_H
#define SOLVER_H

#include "RubiksCube.h"
#include "pattern_databases/PatternDatabase.h"
#include <vector>
#include <iostream>

class Solver {
private:
    static inline int getFace(RubiksCube::MOVE move) { return static_cast<int>(move) / 3; }

    static inline bool areOpposite(int face1, int face2) {
        if (face1 == 0 && face2 == 5) return true;
        if (face1 == 5 && face2 == 0) return true;
        if (face1 == 1 && face2 == 3) return true;
        if (face1 == 3 && face2 == 1) return true;
        if (face1 == 2 && face2 == 4) return true;
        if (face1 == 4 && face2 == 2) return true;
        return false;
    }

    template <typename T>
    static bool dfsIDA(T& cube, int currentDepth, int maxDepth, 
                       int lastFace, int secondLastFace, 
                       PatternDatabase& cornerDB, std::vector<RubiksCube::MOVE>& path) {
        
        // --- THE IDA* CRYSTAL BALL ---
        // If the moves we've made + the minimum moves required for the corners
        // exceeds our depth limit, this branch is physically impossible to solve. Kill it.
        if (currentDepth + cornerDB.getNumMoves(cube) > maxDepth) {
            return false;
        }

        if (currentDepth == maxDepth) {
            return cube.isSolved(); 
        }

        for (int i = 0; i < 18; i++) {
            RubiksCube::MOVE currentMove = static_cast<RubiksCube::MOVE>(i);
            int currentFace = getFace(currentMove);

            // Standard Face Pruning
            if (lastFace != -1) {
                if (currentFace == lastFace) continue;
                if (areOpposite(currentFace, lastFace) && currentFace < lastFace) continue;
                if (secondLastFace != -1 && currentFace == secondLastFace && areOpposite(currentFace, lastFace)) continue;
            }

            cube.move(currentMove);

            if (dfsIDA(cube, currentDepth + 1, maxDepth, currentFace, lastFace, cornerDB, path)) {
                path.insert(path.begin(), currentMove);
                return true; 
            }

            cube.invert(currentMove);
        }
        return false;
    }

    template <typename T>
    static bool dfs(T& cube, int currentDepth, int maxDepth, 
                    int lastFace, int secondLastFace, 
                    std::vector<RubiksCube::MOVE>& path) {
        
        if (currentDepth == maxDepth) {
            return cube.isSolved(); 
        }

        for (int i = 0; i < 18; i++) {
            RubiksCube::MOVE currentMove = static_cast<RubiksCube::MOVE>(i);
            int currentFace = getFace(currentMove);

            // Standard Face Pruning
            if (lastFace != -1) {
                if (currentFace == lastFace) continue;
                if (areOpposite(currentFace, lastFace) && currentFace < lastFace) continue;
                if (secondLastFace != -1 && currentFace == secondLastFace && areOpposite(currentFace, lastFace)) continue;
            }

            cube.move(currentMove);

            if (dfs(cube, currentDepth + 1, maxDepth, currentFace, lastFace, path)) {
                path.insert(path.begin(), currentMove);
                return true; 
            }

            cube.invert(currentMove);
        }
        return false;
    }

public:
    template <typename T>
    static std::vector<RubiksCube::MOVE> solveIDDFS(T& cube, int absoluteMaxDepth = 20) {
        std::vector<RubiksCube::MOVE> path;
        
        std::cout << "Starting pure IDDFS search...\n";

        for (int depth = 0; depth <= absoluteMaxDepth; depth++) {
            std::cout << "Searching depth " << depth << "...\n";
            
            if (dfs(cube, 0, depth, -1, -1, path)) {
                std::cout << "Solution found at depth " << depth << "!\n";
                return path;
            }
        }

        std::cout << "No solution found within " << absoluteMaxDepth << " moves.\n";
        return path;
    }
    template <typename T>
    static std::vector<RubiksCube::MOVE> solveIDAStar(T& cube, PatternDatabase& cornerDB, int absoluteMaxDepth = 20) {
        std::vector<RubiksCube::MOVE> path;
        
        // OPTIMIZATION: Start the search at the exact minimum depth the corners require!
        int initialDepth = cornerDB.getNumMoves(cube);
        std::cout << "Corners require a minimum of " << initialDepth << " moves. Starting search there...\n";

        for (int depth = initialDepth; depth <= absoluteMaxDepth; depth++) {
            std::cout << "Searching depth " << depth << "...\n";
            
            if (dfsIDA(cube, 0, depth, -1, -1, cornerDB, path)) {
                std::cout << "Solution found at depth " << depth << "!\n";
                return path;
            }
        }

        std::cout << "No solution found within " << absoluteMaxDepth << " moves.\n";
        return path;
    }
};

#endif // SOLVER_H