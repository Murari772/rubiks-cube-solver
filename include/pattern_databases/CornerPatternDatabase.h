#ifndef CORNER_PATTERN_DATABASE_H
#define CORNER_PATTERN_DATABASE_H

#include "PatternDatabase.h"

class CornerPatternDatabase : public PatternDatabase {
private:
    static const int FACTORIALS[8];
    
public:
    CornerPatternDatabase();

    uint32_t getDatabaseIndex(const RubiksCube& cube) const override;
};

#endif 