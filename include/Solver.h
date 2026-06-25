#ifndef SOLVER_H
#define SOLVER_H

#include "RubiksCube.h"
#include <vector>

class Solver {
private:
    static bool isRedundant(RubiksCube::MOVE lastMove, RubiksCube::MOVE currentMove);
    static int getFace(RubiksCube::MOVE move);
    static bool areOpposite(int face1, int face2);

    static bool dfs(RubiksCube& cube, int currentDepth, int maxDepth, 
                    RubiksCube::MOVE lastMove, std::vector<RubiksCube::MOVE>& path);

public:
    static std::vector<RubiksCube::MOVE> solveIDDFS(RubiksCube& cube, int maxSearchDepth = 8);
};

#endif 