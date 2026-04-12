#ifndef MAP_H
#define MAP_H

#include <vector>
#include "consts.h"
#include "gradient.h"

const int N = 9;
extern float gridsize;

struct Cell {
	int type;
	float angularVel;
	gradient<float, .05f> highlight;
	gradient<float> shift, size, rotation;
	gradient<float, .1f> innerR, innerG;
	void upd() { 
		highlight.upd(), shift.upd(), size.upd(), innerR.upd(), innerG.upd(), rotation.upd(); 
		rotation.setTar(rotation.getTar() + angularVel);
	}
	Cell():
		type(0), angularVel((.2f + .4f * uniform()) / 120), 
		highlight(0), shift(0), size(0), rotation(0), innerR(0), innerG(0)
	{}
};

struct Map {
	Cell cel[N][N];
	inline Cell* operator[] (int k) { return cel[k]; }
	inline Cell& operator[] (icord c) { return cel[c.x][c.y]; }
	std::pair<int, std::vector<icord>> getPath(icord s, icord t);
	std::pair<int, std::vector<icord>> check();
};

#endif
