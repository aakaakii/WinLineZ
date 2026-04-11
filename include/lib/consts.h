#ifndef CONSTS_H
#define CONSTS_H

#include <math.h>

struct icord {
	int x, y;
	icord() {} icord(int x, int y): x(x), y(y) {}
};
inline bool operator== (const icord& x, const icord& y) { return x.x == y.x && x.y == y.y; }
inline bool operator!= (const icord& x, const icord& y) { return !(x == y); }
inline bool operator< (const icord& x, const icord& y) { return x.x == y.x ? x.y < y.y : x.x < y.x; }

struct fcord {
	float x, y;
	fcord() {} fcord(float x, float y): x(x), y(y) {}
	icord cast() { return {(int)x, (int)y}; }
	fcord(const icord& c): x(c.x), y(c.y) {}
	fcord normalize();
	float length() { return sqrtf(x * x + y * y); }
};
inline fcord operator+ (const fcord& x, const fcord& y) { return {x.x + y.x, x.y + y.y}; }
inline fcord operator- (const fcord& x, const fcord& y) { return {x.x - y.x, x.y - y.y}; }
inline fcord operator* (const fcord& x, float p) { return {x.x * p, x.y * p}; }

int rnd(int l, int r);
float uniform();

#ifdef RAYLIB_H
extern Color ballColor[];
inline Color colorMix(Color x, Color y, float p = .7) {
	return (Color) {
		(unsigned char)(x.r + (y.r - x.r) * p), 
		(unsigned char)(x.g + (y.g - x.g) * p), 
		(unsigned char)(x.b + (y.b - x.b) * p), 
		(unsigned char)(x.a)
	};
}
#endif

#endif
