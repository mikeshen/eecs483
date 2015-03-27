#ifndef H_SCOPER
#define H_SCOPER
#include "tac.h"
typedef enum {UP, DOWN} Growth;
class Scoper {
  protected:
    int off;
    int initOff;
    Growth growth;
    Segment segment;
  public:
    Scoper(Segment s, Growth d);
    Scoper(Scoper *src);
    Location *Alloc(char *name, int size);
    int GetSize();
    // Getters
    int GetOff() { return off; }
    int GetInitOff() { return initOff; }
    Growth GetGrowth() { return growth; }
    Segment GetSegment() { return segment; }
};
/* vim: set ai ts=2 sts=2 sw=2 et: */
#endif /* _H_FRAMEALLOC */
