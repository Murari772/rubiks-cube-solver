#include "../include/Solver.h"
#include <iostream>


bool Solver::isRedundant(RubiksCube::MOVE lastMove, RubiksCube::MOVE currentMove) {
    return (static_cast<int>(lastMove) / 3) == (static_cast<int>(currentMove) / 3);
}

int Solver::getFace(RubiksCube::MOVE move) {
    return static_cast<int>(move) / 3;
}

bool Solver::areOpposite(int face1, int face2) {
    if (face1 == 0 && face2 == 5) return true;
    if (face1 == 5 && face2 == 0) return true;
    if (face1 == 1 && face2 == 3) return true;
    if (face1 == 3 && face2 == 1) return true;
    if (face1 == 2 && face2 == 4) return true;
    if (face1 == 4 && face2 == 2) return true;
    return false;
}


bool Solver::dfs(RubiksCube& cube, int currentDepth, int maxDepth, 
                 RubiksCube::MOVE lastMove, std::vector<RubiksCube::MOVE>& path) {
    
    if (cube.isSolved()) return true;
    if (currentDepth == maxDepth) return false;

    for (int i = 0; i < 18; i++) {
        RubiksCube::MOVE currentMove = static_cast<RubiksCube::MOVE>(i);

        if (!path.empty()) {
            int currentFace = getFace(currentMove);
            int lastFace = getFace(path.back());

            if (currentFace == lastFace) continue;

            if (areOpposite(currentFace, lastFace) && currentFace < lastFace) continue;

            if (path.size() >= 2) {
                int secondLastFace = getFace(path[path.size() - 2]);
                if (currentFace == secondLastFace && areOpposite(currentFace, lastFace)) continue;
            }
        }

        cube.move(currentMove);
        path.push_back(currentMove);

        if (dfs(cube, currentDepth + 1, maxDepth, currentMove, path)) {
            return true; 
        }

        path.pop_back();
        cube.invert(currentMove);
    }

    return false;
}

std::vector<RubiksCube::MOVE> Solver::solveIDDFS(RubiksCube& cube, int maxSearchDepth) {
    std::vector<RubiksCube::MOVE> path;
    
    for (int depth = 1; depth <= maxSearchDepth; depth++) {
        std::cout << "Searching depth " << depth << "...\n";

        if (dfs(cube, 0, depth, RubiksCube::MOVE::U, path)) {
            std::cout << "Solution found at depth " << depth << "!\n";
            return path;
        }
    }

    std::cout << "No solution found within " << maxSearchDepth << " moves.\n";
    return path;
}