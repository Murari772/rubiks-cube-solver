#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <iostream>
#include <vector>
#include <string>

class RubiksCube {
public:
    enum class FACE { UP, LEFT, FRONT, RIGHT, BACK, DOWN };
    enum class COLOR { WHITE, GREEN, RED, BLUE, ORANGE, YELLOW };
    enum class MOVE {
        U, UPRIME, U2,
        L, LPRIME, L2,
        F, FPRIME, F2,
        R, RPRIME, R2,
        B, BPRIME, B2,
        D, DPRIME, D2
    };

    virtual ~RubiksCube() = default;

    // Returns the color at a specific row and column on a face
    virtual COLOR getColor(FACE face, unsigned row, unsigned col) const = 0;

    // Pure virtual function to check if the cube is solved
    virtual bool isSolved() const = 0;

    // Clear string identifier for which model variation is active
    virtual std::string getModelType() const = 0;

    // Helper to print the cube structure in a standard 2D flat cross representation
    void print() const;

    // Execution engine for turning the cube
    virtual RubiksCube& move(MOVE ind) = 0;
    virtual RubiksCube& invert(MOVE ind) = 0;

    // Move Definitions (The 18 core virtual rotations)
    virtual RubiksCube& u() = 0; virtual RubiksCube& uPrime() = 0; virtual RubiksCube& u2() = 0;
    virtual RubiksCube& l() = 0; virtual RubiksCube& lPrime() = 0; virtual RubiksCube& l2() = 0;
    virtual RubiksCube& f() = 0; virtual RubiksCube& fPrime() = 0; virtual RubiksCube& f2() = 0;
    virtual RubiksCube& r() = 0; virtual RubiksCube& rPrime() = 0; virtual RubiksCube& r2() = 0;
    virtual RubiksCube& b() = 0; virtual RubiksCube& bPrime() = 0; virtual RubiksCube& b2() = 0;
    virtual RubiksCube& d() = 0; virtual RubiksCube& dPrime() = 0; virtual RubiksCube& d2() = 0;

    // Helper functions to convert Enums to strings for logging/output
    static char getColorChar(COLOR color);
    static std::string getMoveString(MOVE move);
};

#endif 