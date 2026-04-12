#include "lib/consts.h"
#include <random>
#include <raylib.h>

fcord fcord::normalize() { return *this * (1 / hypot(x, y)); }

static const float magic = 360.f /7;
Color ballColor[] {
	Color{192, 192, 192, 255},
	ColorFromHSV(0*magic, 1, 1),
	ColorFromHSV(1*magic, 1, 1),
	ColorFromHSV(2*magic, 1, 1),
	ColorFromHSV(3*magic, 1, 1),
	ColorFromHSV(4*magic, 1, 1),
	ColorFromHSV(5*magic, 1, 1),
	ColorFromHSV(6*magic, 1, 1),
};

static std::mt19937 _rnd(std::random_device{}());
int rnd(int l, int r) { return _rnd() % (r - l + 1) + l; }
float uniform() { return _rnd() / (1. * _rnd.max()); }
