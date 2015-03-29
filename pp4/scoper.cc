#include "scoper.h"

Scoper::Scoper(Segment s, Growth d) :
    offset(0), initOffset(0), growth(d), segment(s)
{
	if (s == fpRelative && d == DOWN)
		initOffset = offset = -4;
}

Scoper::Scoper(Scoper* src) :
	offset(src->GetOffset()),
	initOffset(src->GetInitOffset()),
	growth(src->GetGrowth()),
	segment(src->GetSegment())
{}

Location* Scoper::Alloc(char* name, int size) {
	if (growth == UP)
		offset += size;
	else
		offset -= size;
	return new Location(segment, offset, name);
}

int Scoper::GetSize() {
	int size = offset - initOffset;
	return (size < 0) ? (size * -1) : size;
}
