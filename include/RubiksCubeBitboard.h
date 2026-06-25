#ifndef RUBIKS_CUBE_BITBOARD_H
#define RUBIKS_CUBE_BITBOARD_H

#include "RubiksCube.h"
#include <cstdint> 

class RubiksCubeBitboard : public RubiksCube {
private:

    uint64_t board[6]{};

    void rotateFaceClockwise(int face);
    void rotateFaceCounterClockwise(int face);

public:
    RubiksCubeBitboard();

    COLOR getColor(FACE face, unsigned row, unsigned col) const override;
    bool isSolved() const override;
    std::string getModelType() const override { return "Bitboard Model"; }

    RubiksCube& move(MOVE ind) override;
    RubiksCube& invert(MOVE ind) override;

    RubiksCube& u() override; RubiksCube& uPrime() override; RubiksCube& u2() override;
    RubiksCube& l() override; RubiksCube& lPrime() override; RubiksCube& l2() override;
    RubiksCube& f() override; RubiksCube& fPrime() override; RubiksCube& f2() override;
    RubiksCube& r() override; RubiksCube& rPrime() override; RubiksCube& r2() override;
    RubiksCube& b() override; RubiksCube& bPrime() override; RubiksCube& b2() override;
    RubiksCube& d() override; RubiksCube& dPrime() override; RubiksCube& d2() override;
};

#endif 