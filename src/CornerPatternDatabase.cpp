#include "../include/pattern_databases/CornerPatternDatabase.h"

const int CornerPatternDatabase::FACTORIALS[8] = { 1, 1, 2, 6, 24, 120, 720, 5040 };

CornerPatternDatabase::CornerPatternDatabase() : PatternDatabase(88179840) {}

namespace {
    typedef RubiksCube::FACE F;
    typedef RubiksCube::COLOR C;

    struct CornerPos {
        F f[3];
        unsigned r[3];
        unsigned c[3];
    };

    const CornerPos corners[8] = {
        {{F::UP, F::LEFT, F::BACK}, {0, 0, 0}, {0, 0, 2}},     // 0: ULB
        {{F::UP, F::BACK, F::RIGHT}, {0, 0, 0}, {2, 0, 2}},    // 1: URB
        {{F::UP, F::RIGHT, F::FRONT}, {2, 0, 0}, {2, 0, 2}},   // 2: URF
        {{F::UP, F::FRONT, F::LEFT}, {2, 0, 0}, {0, 0, 2}},    // 3: ULF
        {{F::DOWN, F::LEFT, F::FRONT}, {0, 2, 2}, {0, 2, 0}},  // 4: DLF
        {{F::DOWN, F::FRONT, F::RIGHT}, {0, 2, 2}, {2, 2, 0}}, // 5: DRF
        {{F::DOWN, F::RIGHT, F::BACK}, {2, 2, 2}, {2, 2, 0}},  // 6: DRB
        {{F::DOWN, F::BACK, F::LEFT}, {2, 2, 2}, {0, 2, 0}},   // 7: DLB
    };

    uint8_t getCornerPiece(C c1, C c2, C c3) {
        bool W = (c1 == C::WHITE || c2 == C::WHITE || c3 == C::WHITE);
        bool Y = (c1 == C::YELLOW || c2 == C::YELLOW || c3 == C::YELLOW);
        bool G = (c1 == C::GREEN || c2 == C::GREEN || c3 == C::GREEN);
        bool B = (c1 == C::BLUE || c2 == C::BLUE || c3 == C::BLUE);
        bool R = (c1 == C::RED || c2 == C::RED || c3 == C::RED);
        bool O = (c1 == C::ORANGE || c2 == C::ORANGE || c3 == C::ORANGE);

        if (W && G && O) return 0;
        if (W && B && O) return 1;
        if (W && B && R) return 2;
        if (W && G && R) return 3;
        if (Y && G && R) return 4;
        if (Y && B && R) return 5;
        if (Y && B && O) return 6;
        if (Y && G && O) return 7;
        return 0; 
    }
}

uint32_t CornerPatternDatabase::getDatabaseIndex(const RubiksCube& cube) const {
    int perm[8];
    int orient[7];
    
    for (int i = 0; i < 8; ++i) {
        C colors[3];
        for (int j = 0; j < 3; ++j) {
            colors[j] = cube.getColor(corners[i].f[j], corners[i].r[j], corners[i].c[j]);
        }
        
        perm[i] = getCornerPiece(colors[0], colors[1], colors[2]);
        
        if (i < 7) {
            if (colors[0] == C::WHITE || colors[0] == C::YELLOW) {
                orient[i] = 0;
            } else if (colors[1] == C::WHITE || colors[1] == C::YELLOW) {
                orient[i] = 1;
            } else {
                orient[i] = 2;
            }
        }
    }

    // 1. Calculate Orientation Index (Base-3)
    uint32_t orientationIndex = 0;
    int base = 1; // 3^0
    for (int i = 6; i >= 0; i--) {
        orientationIndex += orient[i] * base;
        base *= 3;
    }

    // 2. Calculate Permutation Rank (Lehmer Coding)
    uint32_t permutationIndex = 0;
    for (int i = 0; i < 7; i++) {
        int left = 0; // Count pieces to the right that are smaller
        for (int j = i + 1; j < 8; j++) {
            if (perm[j] < perm[i]) {
                left++;
            }
        }
        permutationIndex += left * FACTORIALS[7 - i];
    }

    // 3. Combine them!
    return (permutationIndex * 2187) + orientationIndex;
}