#ifndef RENDER_H
#include <raylib.h>
#include "map.h"
using namespace std;

void InitScreen();
void InitBloom();
void BeginBloomRender();
void EndBloomRender();
void drawCellBox(fcord p, Cell& cell);
void drawCellBall(fcord p, Cell& cell);
void renderBox(fcord pos, Color color, float t=2);
void renderEdge(fcord st, fcord ed, float t, Color color);

// Bloom 参数 (可外部调节)
extern float g_bloomThreshold;
extern float g_bloomIntensity;
extern int g_blurIterations;

#define RENDER_H
#endif
