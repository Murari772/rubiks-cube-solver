#ifndef RUBIKS_CUBE_3D_ARRAY_H
#define RUBIKS_CUBE_3D_ARRAY_H

#include "RubiksCube.h"

class RubiksCube3dArray : public RubiksCube {
private:
    // [Face][Row][Column]
    char cube[6][3][3]{};

    // Helper function to rotate just the 3x3 face itself (ignoring the sides)
    void rotateFaceClockwise(int face);
    void rotateFaceCounterClockwise(int face);

public:
    // Constructor
    RubiksCube3dArray();

    // Overridden Base Class Methods
    COLOR getColor(FACE face, unsigned row, unsigned col) const override;
    bool isSolved() const override;
    std::string getModelType() const override { return "3D Array Model"; }

    // Overridden Move Engines
    RubiksCube& move(MOVE ind) override;
    RubiksCube& invert(MOVE ind) override;

    // The 18 Move implementations
    RubiksCube& u() override; RubiksCube& uPrime() override; RubiksCube& u2() override;
    RubiksCube& l() override; RubiksCube& lPrime() override; RubiksCube& l2() override;
    RubiksCube& f() override; RubiksCube& fPrime() override; RubiksCube& f2() override;
    RubiksCube& r() override; RubiksCube& rPrime() override; RubiksCube& r2() override;
    RubiksCube& b() override; RubiksCube& bPrime() override; RubiksCube& b2() override;
    RubiksCube& d() override; RubiksCube& dPrime() override; RubiksCube& d2() override;
};

#endif // RUBIKS_CUBE_3D_ARRAY_H