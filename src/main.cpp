#include <iostream>
#include <vector>
#include <random>
#include "../include/RubiksCube3dArray.h"

std::vector<RubiksCube::MOVE> generateScramble(int numMoves) {
    std::vector<RubiksCube::MOVE> scramble;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // 6 faces (U=0, L=1, F=2, R=3, B=4, D=5)
    std::uniform_int_distribution<> faceDist(0, 5); 

    // 3 turn types (Clockwise=0, Counter=1, 180-degrees=2)
    std::uniform_int_distribution<> turnDist(0, 2); 

    int lastFace = -1;

    for (int i = 0; i < numMoves; ++i) {
        int currentFace = faceDist(gen);
        
        while (currentFace == lastFace) {
            currentFace = faceDist(gen);
        }
        lastFace = currentFace;

        int turnType = turnDist(gen);
    
        int moveValue = (currentFace * 3) + turnType;
        scramble.push_back(static_cast<RubiksCube::MOVE>(moveValue));
    }
    
    return scramble;
}

int main() {

    RubiksCube3dArray cube;
    std::cout << "Initial Solved State:\n";
    cube.print();

    int scrambleLength = 15;
    std::vector<RubiksCube::MOVE> scrambleSequence = generateScramble(scrambleLength);

    std::cout << "Scrambling with " << scrambleLength << " moves: ";
    for (auto move : scrambleSequence) {
        std::cout << RubiksCube::getMoveString(move) << " ";
        cube.move(move);
    }
    std::cout << "\n\n";
    
    std::cout << "Scrambled State:\n";
    cube.print();

    if (cube.isSolved()) {
        std::cout << "Cube is solved!\n";
    } else {
        std::cout << "Cube is NOT solved.\n";
    }

    return 0;
}