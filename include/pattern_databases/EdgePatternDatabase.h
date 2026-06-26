#ifndef EDGE_PATTERN_DATABASE_H
#define EDGE_PATTERN_DATABASE_H

#include "PatternDatabase.h"

class EdgePatternDatabase : public PatternDatabase {
private:
    int startIndex;
public:
    // Tracks 6 edges starting from startIdx (0 for edges 0-5, 6 for edges 6-11)
    EdgePatternDatabase(int startIdx);

    uint32_t getDatabaseIndex(const RubiksCube& cube) const override;
};

#endif // EDGE_PATTERN_DATABASE_H
