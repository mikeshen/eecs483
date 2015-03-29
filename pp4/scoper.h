#ifndef H_SCOPER
#define H_SCOPER
#include "tac.h"

enum Growth {UP, DOWN};

class Scoper {
protected:
    int offset;
    int initOffset;
    Growth growth;
    Segment segment;
public:
    Scoper(Segment s, Growth d);
    Scoper(Scoper *src);
    Location *Alloc(char *name, int size);
    int GetSize();
    // Getters
    int GetOffset() { return offset; }
    int GetInitOffset() { return initOffset; }
    Growth GetGrowth() { return growth; }
    Segment GetSegment() { return segment; }
};

#endif /* _H_FRAMEALLOC */
