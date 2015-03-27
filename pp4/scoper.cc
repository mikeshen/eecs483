#include "scoper.h"
Scoper::Scoper(Segment s, Growth d) {
	off = initOff = 0;
	if (s == fpRelative && d == DOWN) {
		initOff = off = -4;
	}
	growth = d;
	segment = s;
}
Scoper::Scoper(Scoper *src) {
	off = src->GetOff();
	initOff = src->GetInitOff();
	growth = src->GetGrowth();
	segment = src->GetSegment();
}
Location *Scoper::Alloc(char *name, int size) {
	if (growth == UP) {
		off += size;
	} else {
		off -= size;
	}
	return new Location(segment, off, name);
}
int Scoper::GetSize() {
	int size = off - initOff;
	return (size < 0) ? (size * -1) : size;
}
