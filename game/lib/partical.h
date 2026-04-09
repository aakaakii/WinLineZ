#ifndef PARTICAL_H
#define PARTICAL_H
#include "consts.h"
#include "gradient.h"

struct Partical {
	fcord pos;
	fcord vel; 
	float size;
	int color;
	float vlen;
	
	void upd() {
		pos = pos + vel;
		vel = vel * .98;
		if(vel.length() < .1) {
			size *= .95; 
		}
	}
};

#endif
