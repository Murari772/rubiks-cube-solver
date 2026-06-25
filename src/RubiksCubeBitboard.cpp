#include "../include/RubiksCubeBitboard.h"

namespace {
    uint64_t getTopRow(const uint64_t board[], int face) {
        uint64_t s0 = board[face] & 0b111;
        uint64_t s1 = (board[face] >> 3) & 0b111;
        uint64_t s2 = (board[face] >> 6) & 0b111;
        return s0 | (s1 << 3) | (s2 << 6);
    }

    void setTopRow(uint64_t board[], int face, uint64_t slice) {
        board[face] = (board[face] & ~0x1FFULL) | (slice & 0x1FFULL);
    }

    uint64_t getBottomRow(const uint64_t board[], int face) {
        uint64_t s0 = (board[face] >> 18) & 0b111; 
        uint64_t s1 = (board[face] >> 15) & 0b111; 
        uint64_t s2 = (board[face] >> 12) & 0b111; 
        return s0 | (s1 << 3) | (s2 << 6);
    }

    void setBottomRow(uint64_t board[], int face, uint64_t slice) {
        uint64_t s0 = slice & 0b111;
        uint64_t s1 = (slice >> 3) & 0b111;
        uint64_t s2 = (slice >> 6) & 0b111;
        uint64_t mask = (0b111ULL << 18) | (0b111ULL << 15) | (0b111ULL << 12);
        board[face] = (board[face] & ~mask) | (s0 << 18) | (s1 << 15) | (s2 << 12);
    }

    uint64_t getLeftCol(const uint64_t board[], int face) {
        uint64_t s0 = board[face] & 0b111; 
        uint64_t s1 = (board[face] >> 21) & 0b111; 
        uint64_t s2 = (board[face] >> 18) & 0b111; 
        return s0 | (s1 << 3) | (s2 << 6);
    }

    void setLeftCol(uint64_t board[], int face, uint64_t slice) {
        uint64_t s0 = slice & 0b111;
        uint64_t s1 = (slice >> 3) & 0b111;
        uint64_t s2 = (slice >> 6) & 0b111;
        uint64_t mask = 0b111ULL | (0b111ULL << 21) | (0b111ULL << 18);
        board[face] = (board[face] & ~mask) | s0 | (s1 << 21) | (s2 << 18);
    }

    uint64_t getRightCol(const uint64_t board[], int face) {
        uint64_t s0 = (board[face] >> 6) & 0b111; 
        uint64_t s1 = (board[face] >> 9) & 0b111; 
        uint64_t s2 = (board[face] >> 12) & 0b111; 
        return s0 | (s1 << 3) | (s2 << 6);
    }

    void setRightCol(uint64_t board[], int face, uint64_t slice) {
        uint64_t s0 = slice & 0b111;
        uint64_t s1 = (slice >> 3) & 0b111;
        uint64_t s2 = (slice >> 6) & 0b111;
        uint64_t mask = (0b111ULL << 6) | (0b111ULL << 9) | (0b111ULL << 12);
        board[face] = (board[face] & ~mask) | (s0 << 6) | (s1 << 9) | (s2 << 12);
    }

    uint64_t invertSlice(uint64_t slice) {
        uint64_t s0 = slice & 0b111;
        uint64_t s1 = (slice >> 3) & 0b111;
        uint64_t s2 = (slice >> 6) & 0b111;
        return s2 | (s1 << 3) | (s0 << 6);
    }
}

RubiksCubeBitboard::RubiksCubeBitboard() {
    for (int face = 0; face < 6; ++face) {
        board[face] = 0; 
        uint64_t color = face; 

        for (int sticker = 0; sticker < 8; ++sticker) {
            board[face] |= (color << (sticker * 3));
        }
    }
}

int getStickerIndex(unsigned row, unsigned col) {
    if (row == 0 && col == 0) return 0; // Top-Left
    if (row == 0 && col == 1) return 1; // Top-Middle
    if (row == 0 && col == 2) return 2; // Top-Right
    if (row == 1 && col == 2) return 3; // Right-Middle
    if (row == 2 && col == 2) return 4; // Bottom-Right
    if (row == 2 && col == 1) return 5; // Bottom-Middle
    if (row == 2 && col == 0) return 6; // Bottom-Left
    if (row == 1 && col == 0) return 7; // Left-Middle
    return -1; // Center Piece
}

RubiksCube::COLOR RubiksCubeBitboard::getColor(FACE face, unsigned row, unsigned col) const {
    int stickerIdx = getStickerIndex(row, col);
    int faceIdx = static_cast<int>(face);
    
    if (stickerIdx == -1) {
        return static_cast<COLOR>(faceIdx);
    }
    
    uint64_t colorVal = (board[faceIdx] >> (stickerIdx * 3)) & 0b111;
    return static_cast<COLOR>(colorVal);
}

bool RubiksCubeBitboard::isSolved() const {
    for (int face = 0; face < 6; face++) {
        uint64_t solvedState = 0;
        uint64_t color = face;
        for (int sticker = 0; sticker < 8; ++sticker) {
            solvedState |= (color << (sticker * 3));
        }
        if (board[face] != solvedState) return false;
    }
    return true;
}

void RubiksCubeBitboard::rotateFaceClockwise(int face) {
    uint64_t mask24 = 0xFFFFFF;
    board[face] = ((board[face] << 6) & mask24) | (board[face] >> 18);
}

void RubiksCubeBitboard::rotateFaceCounterClockwise(int face) {
    uint64_t mask24 = 0xFFFFFF;
    board[face] = ((board[face] >> 6) | (board[face] << 18)) & mask24;
}

RubiksCube& RubiksCubeBitboard::move(MOVE ind) {
    switch (ind) {
        case MOVE::U: return this->u(); case MOVE::UPRIME: return this->uPrime(); case MOVE::U2: return this->u2();
        case MOVE::L: return this->l(); case MOVE::LPRIME: return this->lPrime(); case MOVE::L2: return this->l2();
        case MOVE::F: return this->f(); case MOVE::FPRIME: return this->fPrime(); case MOVE::F2: return this->f2();
        case MOVE::R: return this->r(); case MOVE::RPRIME: return this->rPrime(); case MOVE::R2: return this->r2();
        case MOVE::B: return this->b(); case MOVE::BPRIME: return this->bPrime(); case MOVE::B2: return this->b2();
        case MOVE::D: return this->d(); case MOVE::DPRIME: return this->dPrime(); case MOVE::D2: return this->d2();
        default: return *this;
    }
}

RubiksCube& RubiksCubeBitboard::invert(MOVE ind) {
    switch (ind) {
        case MOVE::U: return this->uPrime(); case MOVE::UPRIME: return this->u(); case MOVE::U2: return this->u2();
        case MOVE::L: return this->lPrime(); case MOVE::LPRIME: return this->l(); case MOVE::L2: return this->l2();
        case MOVE::F: return this->fPrime(); case MOVE::FPRIME: return this->f(); case MOVE::F2: return this->f2();
        case MOVE::R: return this->rPrime(); case MOVE::RPRIME: return this->r(); case MOVE::R2: return this->r2();
        case MOVE::B: return this->bPrime(); case MOVE::BPRIME: return this->b(); case MOVE::B2: return this->b2();
        case MOVE::D: return this->dPrime(); case MOVE::DPRIME: return this->d(); case MOVE::D2: return this->d2();
        default: return *this;
    }
}

// --- UP ---
RubiksCube& RubiksCubeBitboard::u() {
    rotateFaceClockwise(0); 
    uint64_t temp = getTopRow(board, 4);
    setTopRow(board, 4, getTopRow(board, 1));
    setTopRow(board, 1, getTopRow(board, 2));
    setTopRow(board, 2, getTopRow(board, 3));
    setTopRow(board, 3, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::uPrime() {
    rotateFaceCounterClockwise(0);
    uint64_t temp = getTopRow(board, 4);
    setTopRow(board, 4, getTopRow(board, 3));
    setTopRow(board, 3, getTopRow(board, 2));
    setTopRow(board, 2, getTopRow(board, 1));
    setTopRow(board, 1, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::u2() { 
    this->u(); 
    return this->u(); 
}

// --- LEFT ---
RubiksCube& RubiksCubeBitboard::l() {
    rotateFaceClockwise(1);
    uint64_t temp = getLeftCol(board, 0);
    setLeftCol(board, 0, invertSlice(getRightCol(board, 4)));
    setRightCol(board, 4, invertSlice(getLeftCol(board, 5)));
    setLeftCol(board, 5, getLeftCol(board, 2));
    setLeftCol(board, 2, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::lPrime() {
    rotateFaceCounterClockwise(1);
    uint64_t temp = getLeftCol(board, 0);
    setLeftCol(board, 0, getLeftCol(board, 2));
    setLeftCol(board, 2, getLeftCol(board, 5));
    setLeftCol(board, 5, invertSlice(getRightCol(board, 4)));
    setRightCol(board, 4, invertSlice(temp));
    return *this;
}

RubiksCube& RubiksCubeBitboard::l2() { 
    this->l(); 
    return this->l();
}

// --- FRONT ---
RubiksCube& RubiksCubeBitboard::f() {
    rotateFaceClockwise(2);
    uint64_t temp = getBottomRow(board, 0);
    setBottomRow(board, 0, invertSlice(getRightCol(board, 1)));
    setRightCol(board, 1, getTopRow(board, 5));
    setTopRow(board, 5, invertSlice(getLeftCol(board, 3)));
    setLeftCol(board, 3, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::fPrime() {
    rotateFaceCounterClockwise(2);
    uint64_t temp = getBottomRow(board, 0);
    setBottomRow(board, 0, getLeftCol(board, 3));
    setLeftCol(board, 3, invertSlice(getTopRow(board, 5)));
    setTopRow(board, 5, getRightCol(board, 1));
    setRightCol(board, 1, invertSlice(temp));
    return *this;
}

RubiksCube& RubiksCubeBitboard::f2() { 
    this->f(); 
    return this->f();
}

// --- RIGHT ---
RubiksCube& RubiksCubeBitboard::r() {
    rotateFaceClockwise(3);
    uint64_t temp = getRightCol(board, 0);
    setRightCol(board, 0, getRightCol(board, 2));
    setRightCol(board, 2, getRightCol(board, 5));
    setRightCol(board, 5, invertSlice(getLeftCol(board, 4)));
    setLeftCol(board, 4, invertSlice(temp));
    return *this;
}

RubiksCube& RubiksCubeBitboard::rPrime() {
    rotateFaceCounterClockwise(3);
    uint64_t temp = getRightCol(board, 0);
    setRightCol(board, 0, invertSlice(getLeftCol(board, 4)));
    setLeftCol(board, 4, invertSlice(getRightCol(board, 5)));
    setRightCol(board, 5, getRightCol(board, 2));
    setRightCol(board, 2, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::r2() { 
    this->r(); 
    return this->r();
}

// --- BACK ---
RubiksCube& RubiksCubeBitboard::b() {
    rotateFaceClockwise(4);
    uint64_t temp = getTopRow(board, 0);
    setTopRow(board, 0, getRightCol(board, 3));
    setRightCol(board, 3, invertSlice(getBottomRow(board, 5)));
    setBottomRow(board, 5, getLeftCol(board, 1));
    setLeftCol(board, 1, invertSlice(temp));
    return *this;
}

RubiksCube& RubiksCubeBitboard::bPrime() {
    rotateFaceCounterClockwise(4);
    uint64_t temp = getTopRow(board, 0);
    setTopRow(board, 0, invertSlice(getLeftCol(board, 1)));
    setLeftCol(board, 1, getBottomRow(board, 5));
    setBottomRow(board, 5, invertSlice(getRightCol(board, 3)));
    setRightCol(board, 3, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::b2() { 
    this->b();
    return this->b();  
}

// --- DOWN ---
RubiksCube& RubiksCubeBitboard::d() {
    rotateFaceClockwise(5);
    uint64_t temp = getBottomRow(board, 2);
    setBottomRow(board, 2, getBottomRow(board, 1));
    setBottomRow(board, 1, getBottomRow(board, 4));
    setBottomRow(board, 4, getBottomRow(board, 3));
    setBottomRow(board, 3, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::dPrime() {
    rotateFaceCounterClockwise(5);
    uint64_t temp = getBottomRow(board, 2);
    setBottomRow(board, 2, getBottomRow(board, 3));
    setBottomRow(board, 3, getBottomRow(board, 4));
    setBottomRow(board, 4, getBottomRow(board, 1));
    setBottomRow(board, 1, temp);
    return *this;
}

RubiksCube& RubiksCubeBitboard::d2() { 
    this->d(); 
    return this->d(); 
}