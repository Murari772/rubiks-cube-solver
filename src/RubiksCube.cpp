#include "../include/RubiksCube.h"

char RubiksCube::getColorChar(COLOR color) {
    switch (color) {
        case COLOR::WHITE:  return 'W';
        case COLOR::GREEN:  return 'G';
        case COLOR::RED:    return 'R';
        case COLOR::BLUE:   return 'B';
        case COLOR::ORANGE: return 'O';
        case COLOR::YELLOW: return 'Y';
    }
    return 'X';
}

std::string RubiksCube::getMoveString(MOVE move) {
    switch (move) {
        case MOVE::U: return "U"; case MOVE::UPRIME: return "U'"; case MOVE::U2: return "U2";
        case MOVE::L: return "L"; case MOVE::LPRIME: return "L'"; case MOVE::L2: return "L2";
        case MOVE::F: return "F"; case MOVE::FPRIME: return "F'"; case MOVE::F2: return "F2";
        case MOVE::R: return "R"; case MOVE::RPRIME: return "R'"; case MOVE::R2: return "R2";
        case MOVE::B: return "B"; case MOVE::BPRIME: return "B'"; case MOVE::B2: return "B2";
        case MOVE::D: return "D"; case MOVE::DPRIME: return "D'"; case MOVE::D2: return "D2";
    }
    return "";
}

void RubiksCube::print() const {
    // 1. Print Up Face
    for (int r = 0; r < 3; ++r) {
        std::cout << "      ";
        for (int c = 0; c < 3; ++c) std::cout << getColorChar(getColor(FACE::UP, r, c)) << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    // 2. Print Left, Front, Right, Back side-by-side
    for (int r = 0; r < 3; ++r) {
        // Left
        for (int c = 0; c < 3; ++c) std::cout << getColorChar(getColor(FACE::LEFT, r, c)) << " ";
        std::cout << " ";

        // Front
        for (int c = 0; c < 3; ++c) std::cout << getColorChar(getColor(FACE::FRONT, r, c)) << " ";
        std::cout << " ";

        // Right
        for (int c = 0; c < 3; ++c) std::cout << getColorChar(getColor(FACE::RIGHT, r, c)) << " ";
        std::cout << " ";

        // Back
        for (int c = 0; c < 3; ++c) std::cout << getColorChar(getColor(FACE::BACK, r, c)) << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    // 3. Print Down Face
    for (int r = 0; r < 3; ++r) {
        std::cout << "      ";
        
        for (int c = 0; c < 3; ++c) std::cout << getColorChar(getColor(FACE::DOWN, r, c)) << " ";
        std::cout << "\n";
    }

    std::cout << "\n";
}