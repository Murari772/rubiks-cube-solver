#include "../include/pattern_databases/EdgePatternDatabase.h"

EdgePatternDatabase::EdgePatternDatabase(int startIdx) : PatternDatabase(42577920), startIndex(startIdx) {}

namespace {
    typedef RubiksCube::FACE F;
    typedef RubiksCube::COLOR C;

    struct EdgePos {
        F f[2];
        unsigned r[2];
        unsigned c[2];
    };

    const EdgePos edges[12] = {
        {{F::UP, F::BACK}, {0, 0}, {1, 1}},     // 0: UB
        {{F::UP, F::LEFT}, {1, 0}, {0, 1}},     // 1: UL
        {{F::UP, F::RIGHT}, {1, 0}, {2, 1}},    // 2: UR
        {{F::UP, F::FRONT}, {2, 0}, {1, 1}},    // 3: UF
        {{F::BACK, F::LEFT}, {1, 1}, {2, 0}},   // 4: BL
        {{F::BACK, F::RIGHT}, {1, 1}, {0, 2}},  // 5: BR
        {{F::FRONT, F::LEFT}, {1, 1}, {0, 2}},  // 6: FL
        {{F::FRONT, F::RIGHT}, {1, 1}, {2, 0}}, // 7: FR
        {{F::DOWN, F::BACK}, {2, 2}, {1, 1}},   // 8: DB
        {{F::DOWN, F::LEFT}, {1, 2}, {0, 1}},   // 9: DL
        {{F::DOWN, F::RIGHT}, {1, 2}, {2, 1}},  // 10: DR
        {{F::DOWN, F::FRONT}, {0, 2}, {1, 1}},  // 11: DF
    };

    uint8_t getEdgePiece(C c1, C c2) {
        bool W = (c1 == C::WHITE || c2 == C::WHITE);
        bool Y = (c1 == C::YELLOW || c2 == C::YELLOW);
        bool G = (c1 == C::GREEN || c2 == C::GREEN);
        bool B = (c1 == C::BLUE || c2 == C::BLUE);
        bool R = (c1 == C::RED || c2 == C::RED);
        bool O = (c1 == C::ORANGE || c2 == C::ORANGE);

        if (W && O) return 0;
        if (W && G) return 1;
        if (W && B) return 2;
        if (W && R) return 3;
        if (O && G) return 4;
        if (O && B) return 5;
        if (R && G) return 6;
        if (R && B) return 7;
        if (Y && O) return 8;
        if (Y && G) return 9;
        if (Y && B) return 10;
        if (Y && R) return 11;
        return 0; 
    }

    uint8_t getEdgeOrientation(C c0, C c1) {
        if (c0 == C::WHITE || c0 == C::YELLOW) return 0;
        if (c1 == C::WHITE || c1 == C::YELLOW) return 1;
        
        if (c0 == C::GREEN || c0 == C::BLUE) return 0;
        if (c1 == C::GREEN || c1 == C::BLUE) return 1;
        
        return 0;
    }
}

uint32_t EdgePatternDatabase::getDatabaseIndex(const RubiksCube& cube) const {
    int edgeSlots[6]; 
    int orient[6];

    for (int i = 0; i < 12; ++i) {
        C colors[2];
        colors[0] = cube.getColor(edges[i].f[0], edges[i].r[0], edges[i].c[0]);
        colors[1] = cube.getColor(edges[i].f[1], edges[i].r[1], edges[i].c[1]);
        
        uint8_t piece = getEdgePiece(colors[0], colors[1]);
        
        if (piece >= startIndex && piece < startIndex + 6) {
            int trackIndex = piece - startIndex;
            edgeSlots[trackIndex] = i;
            orient[trackIndex] = getEdgeOrientation(colors[0], colors[1]);
        }
    }

    uint32_t orientationHash = 0;
    for (int i = 0; i < 6; ++i) {
        orientationHash += orient[i] << i;
    }

    const int WEIGHTS[6] = {55440, 5040, 504, 56, 7, 1};
    uint32_t permutationRank = 0;
    bool available[12];
    for (int i = 0; i < 12; ++i) available[i] = true;

    for (int i = 0; i < 6; ++i) {
        int slot = edgeSlots[i];
        int rank = 0;
        for (int j = 0; j < slot; ++j) {
            if (available[j]) rank++;
        }
        permutationRank += rank * WEIGHTS[i];
        available[slot] = false;
    }

    return (permutationRank * 64) + orientationHash;
}
