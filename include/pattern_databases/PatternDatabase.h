#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include "../RubiksCube.h"
#include <vector>
#include <string>
#include <cstdint>

class PatternDatabase {
protected:
    std::vector<uint8_t> database; 
    size_t size;
    size_t numElements;

public:
    PatternDatabase(size_t size);
    virtual ~PatternDatabase() = default;

    virtual uint32_t getDatabaseIndex(const RubiksCube& cube) const = 0;

    bool setNumMoves(const RubiksCube& cube, uint8_t numMoves);
    bool setNumMoves(uint32_t index, uint8_t numMoves);
    uint8_t getNumMoves(const RubiksCube& cube) const;
    uint8_t getNumMoves(uint32_t index) const;

    size_t getSize() const { return size; }
    size_t getNumElements() const { return numElements; }
    bool isFull() const { return numElements == size; }

    virtual bool loadDatabase(const std::string& filePath);
    virtual bool saveDatabase(const std::string& filePath) const;
};

#endif 