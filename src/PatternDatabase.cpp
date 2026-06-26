#include "../include/pattern_databases/PatternDatabase.h"
#include <fstream>
#include <iostream>

PatternDatabase::PatternDatabase(size_t size) : size(size), numElements(0) {
    database.assign(size, 0xFF);
}

bool PatternDatabase::setNumMoves(const RubiksCube& cube, uint8_t numMoves) {
    return setNumMoves(getDatabaseIndex(cube), numMoves);
}

bool PatternDatabase::setNumMoves(uint32_t index, uint8_t numMoves) {
    if (database[index] == 0xFF) {
        database[index] = numMoves;
        numElements++;
        return true;
    }
    return false;
}

uint8_t PatternDatabase::getNumMoves(const RubiksCube& cube) const {
    return getNumMoves(getDatabaseIndex(cube));
}

uint8_t PatternDatabase::getNumMoves(uint32_t index) const {
    return database[index];
}

bool PatternDatabase::loadDatabase(const std::string& filePath) {
    std::ifstream reader(filePath, std::ios::in | std::ios::binary);
    if (!reader.is_open()) return false;

    reader.read(reinterpret_cast<char*>(database.data()), size);
    
    numElements = 0;
    for (uint8_t moves : database) {
        if (moves != 0xFF) numElements++;
    }

    reader.close();
    std::cout << "Loaded Pattern Database: " << numElements << " states.\n";
    return true;
}

bool PatternDatabase::saveDatabase(const std::string& filePath) const {
    std::ofstream writer(filePath, std::ios::out | std::ios::binary);
    if (!writer.is_open()) return false;

    writer.write(reinterpret_cast<const char*>(database.data()), size);
    writer.close();
    
    std::cout << "Saved Pattern Database: " << numElements << " states.\n";
    return true;
}