#include "../include/RubiksCube3dArray.h"


RubiksCube3dArray::RubiksCube3dArray() {
    // 0:UP(W), 1:LEFT(G), 2:FRONT(R), 3:RIGHT(B), 4:BACK(O), 5:DOWN(Y)
    char colors[6] = {'W', 'G', 'R', 'B', 'O', 'Y'};
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                cube[face][row][col] = colors[face];
            }
        }
    }
}

RubiksCube::COLOR RubiksCube3dArray::getColor(FACE face, unsigned row, unsigned col) const {
    char color = cube[static_cast<int>(face)][row][col];
    switch (color) {
        case 'W': return COLOR::WHITE;
        case 'G': return COLOR::GREEN;
        case 'R': return COLOR::RED;
        case 'B': return COLOR::BLUE;
        case 'O': return COLOR::ORANGE;
        case 'Y': return COLOR::YELLOW;
    }

    return COLOR::WHITE; 
}

bool RubiksCube3dArray::isSolved() const {
    for (int face = 0; face < 6; face++) {
        char centerColor = cube[face][1][1];
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (cube[face][row][col] != centerColor) return false;
            }
        }
    }

    return true;
}

void RubiksCube3dArray::rotateFaceClockwise(int face) {
    char temp[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[j][2 - i] = cube[face][i][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cube[face][i][j] = temp[i][j];
        }
    }
}

void RubiksCube3dArray::rotateFaceCounterClockwise(int face) {
    char temp[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[2 - j][i] = cube[face][i][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cube[face][i][j] = temp[i][j];
        }
    }
}


RubiksCube& RubiksCube3dArray::move(MOVE ind) {
    switch (ind) {
        case MOVE::U: return this->u(); case MOVE::UPRIME: return this->uPrime(); case MOVE::U2: return this->u2();
        case MOVE::L: return this->l(); case MOVE::LPRIME: return this->lPrime(); case MOVE::L2: return this->l2();
        case MOVE::F: return this->f(); case MOVE::FPRIME: return this->fPrime(); case MOVE::F2: return this->f2();
        case MOVE::R: return this->r(); case MOVE::RPRIME: return this->rPrime(); case MOVE::R2: return this->r2();
        case MOVE::B: return this->b(); case MOVE::BPRIME: return this->bPrime(); case MOVE::B2: return this->b2();
        case MOVE::D: return this->d(); case MOVE::DPRIME: return this->dPrime(); case MOVE::D2: return this->d2();
    }

    return *this;
}

RubiksCube& RubiksCube3dArray::invert(MOVE ind) {
    switch (ind) {
        case MOVE::U: return this->uPrime(); case MOVE::UPRIME: return this->u(); case MOVE::U2: return this->u2();
        case MOVE::L: return this->lPrime(); case MOVE::LPRIME: return this->l(); case MOVE::L2: return this->l2();
        case MOVE::F: return this->fPrime(); case MOVE::FPRIME: return this->f(); case MOVE::F2: return this->f2();
        case MOVE::R: return this->rPrime(); case MOVE::RPRIME: return this->r(); case MOVE::R2: return this->r2();
        case MOVE::B: return this->bPrime(); case MOVE::BPRIME: return this->b(); case MOVE::B2: return this->b2();
        case MOVE::D: return this->dPrime(); case MOVE::DPRIME: return this->d(); case MOVE::D2: return this->d2();
    }

    return *this;
}

// --- UP ---
RubiksCube& RubiksCube3dArray::u() {
    rotateFaceClockwise(0);
    char temp[3];
    for (int i = 0; i < 3; i++) temp[i] = cube[4][0][i]; // Save Back
    for (int i = 0; i < 3; i++) cube[4][0][i] = cube[1][0][i]; // Left -> Back
    for (int i = 0; i < 3; i++) cube[1][0][i] = cube[2][0][i]; // Front -> Left
    for (int i = 0; i < 3; i++) cube[2][0][i] = cube[3][0][i]; // Right -> Front
    for (int i = 0; i < 3; i++) cube[3][0][i] = temp[i];       // Back -> Right
    return *this;
}

RubiksCube& RubiksCube3dArray::uPrime() {
    rotateFaceCounterClockwise(0);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[4][0][i]; 
    for (int i = 0; i < 3; i++) cube[4][0][i] = cube[3][0][i]; 
    for (int i = 0; i < 3; i++) cube[3][0][i] = cube[2][0][i]; 
    for (int i = 0; i < 3; i++) cube[2][0][i] = cube[1][0][i]; 
    for (int i = 0; i < 3; i++) cube[1][0][i] = temp[i];  

    return *this;
}

RubiksCube& RubiksCube3dArray::u2() { 
    this->u(); 
    this->u();

    return *this; 
}

// --- LEFT ---
RubiksCube& RubiksCube3dArray::l() {
    rotateFaceClockwise(1);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][i][0]; // Save Up
    for (int i = 0; i < 3; i++) cube[0][i][0] = cube[4][2 - i][2]; // Back -> Up (inverted)
    for (int i = 0; i < 3; i++) cube[4][2 - i][2] = cube[5][i][0]; // Down -> Back (inverted)
    for (int i = 0; i < 3; i++) cube[5][i][0] = cube[2][i][0];     // Front -> Down
    for (int i = 0; i < 3; i++) cube[2][i][0] = temp[i];           // Up -> Front

    return *this;
}

RubiksCube& RubiksCube3dArray::lPrime() {
    rotateFaceCounterClockwise(1);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][i][0]; 
    for (int i = 0; i < 3; i++) cube[0][i][0] = cube[2][i][0];     
    for (int i = 0; i < 3; i++) cube[2][i][0] = cube[5][i][0];     
    for (int i = 0; i < 3; i++) cube[5][i][0] = cube[4][2 - i][2]; 
    for (int i = 0; i < 3; i++) cube[4][2 - i][2] = temp[i];   

    return *this;
}

RubiksCube& RubiksCube3dArray::l2() { 
    this->l();
    this->l();

    return *this; 
}

// --- FRONT ---
RubiksCube& RubiksCube3dArray::f() {
    rotateFaceClockwise(2);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][2][i]; // Save Up
    for (int i = 0; i < 3; i++) cube[0][2][i] = cube[1][2 - i][2]; // Left -> Up
    for (int i = 0; i < 3; i++) cube[1][2 - i][2] = cube[5][0][2 - i]; // Down -> Left
    for (int i = 0; i < 3; i++) cube[5][0][2 - i] = cube[3][i][0];     // Right -> Down
    for (int i = 0; i < 3; i++) cube[3][i][0] = temp[i];               // Up -> Right

    return *this;
}

RubiksCube& RubiksCube3dArray::fPrime() {
    rotateFaceCounterClockwise(2);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][2][i]; 
    for (int i = 0; i < 3; i++) cube[0][2][i] = cube[3][i][0];               
    for (int i = 0; i < 3; i++) cube[3][i][0] = cube[5][0][2 - i];     
    for (int i = 0; i < 3; i++) cube[5][0][2 - i] = cube[1][2 - i][2]; 
    for (int i = 0; i < 3; i++) cube[1][2 - i][2] = temp[i]; 

    return *this;
}

RubiksCube& RubiksCube3dArray::f2() { 
    this->f(); 
    this->f();

    return *this; 
}

// --- RIGHT ---
RubiksCube& RubiksCube3dArray::r() {
    rotateFaceClockwise(3);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][i][2]; // Save Up
    for (int i = 0; i < 3; i++) cube[0][i][2] = cube[2][i][2];         // Front -> Up
    for (int i = 0; i < 3; i++) cube[2][i][2] = cube[5][i][2];         // Down -> Front
    for (int i = 0; i < 3; i++) cube[5][i][2] = cube[4][2 - i][0];     // Back -> Down (inverted)
    for (int i = 0; i < 3; i++) cube[4][2 - i][0] = temp[i];           // Up -> Back (inverted)

    return *this;
}

RubiksCube& RubiksCube3dArray::rPrime() {
    rotateFaceCounterClockwise(3);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][i][2]; 
    for (int i = 0; i < 3; i++) cube[0][i][2] = cube[4][2 - i][0];           
    for (int i = 0; i < 3; i++) cube[4][2 - i][0] = cube[5][i][2];     
    for (int i = 0; i < 3; i++) cube[5][i][2] = cube[2][i][2];         
    for (int i = 0; i < 3; i++) cube[2][i][2] = temp[i]; 

    return *this;
}

RubiksCube& RubiksCube3dArray::r2() { 
    this->r(); 
    this->r();

    return *this; 
}

// --- BACK ---
RubiksCube& RubiksCube3dArray::b() {
    rotateFaceClockwise(4);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][0][i]; // Save Up
    for (int i = 0; i < 3; i++) cube[0][0][i] = cube[3][i][2];             // Right -> Up
    for (int i = 0; i < 3; i++) cube[3][i][2] = cube[5][2][2 - i];         // Down -> Right
    for (int i = 0; i < 3; i++) cube[5][2][2 - i] = cube[1][2 - i][0];     // Left -> Down
    for (int i = 0; i < 3; i++) cube[1][2 - i][0] = temp[i];               // Up -> Left
    
    return *this;
}

RubiksCube& RubiksCube3dArray::bPrime() {
    rotateFaceCounterClockwise(4);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[0][0][i]; 
    for (int i = 0; i < 3; i++) cube[0][0][i] = cube[1][2 - i][0];               
    for (int i = 0; i < 3; i++) cube[1][2 - i][0] = cube[5][2][2 - i];     
    for (int i = 0; i < 3; i++) cube[5][2][2 - i] = cube[3][i][2];         
    for (int i = 0; i < 3; i++) cube[3][i][2] = temp[i];  

    return *this;
}

RubiksCube& RubiksCube3dArray::b2() { 
    this->b();
    this->b();  
    return *this; 
}

// --- DOWN ---
RubiksCube& RubiksCube3dArray::d() {
    rotateFaceClockwise(5);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[2][2][i]; // Save Front
    for (int i = 0; i < 3; i++) cube[2][2][i] = cube[1][2][i]; // Left -> Front
    for (int i = 0; i < 3; i++) cube[1][2][i] = cube[4][2][i]; // Back -> Left
    for (int i = 0; i < 3; i++) cube[4][2][i] = cube[3][2][i]; // Right -> Back
    for (int i = 0; i < 3; i++) cube[3][2][i] = temp[i];       // Front -> Right

    return *this;
}

RubiksCube& RubiksCube3dArray::dPrime() {
    rotateFaceCounterClockwise(5);
    char temp[3];

    for (int i = 0; i < 3; i++) temp[i] = cube[2][2][i]; 
    for (int i = 0; i < 3; i++) cube[2][2][i] = cube[3][2][i];       
    for (int i = 0; i < 3; i++) cube[3][2][i] = cube[4][2][i]; 
    for (int i = 0; i < 3; i++) cube[4][2][i] = cube[1][2][i]; 
    for (int i = 0; i < 3; i++) cube[1][2][i] = temp[i]; 

    return *this;
}
RubiksCube& RubiksCube3dArray::d2() { 
    this->d(); 
    this->d(); 
    return *this; 
}