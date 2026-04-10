#include "lib/render.h"
#include "game.h"
#include "lib/map.h"
#include "lib/partical.h"
#include <math.h>
#include <stdio.h>

static const int ballCnt = 3;

void Game::init() {
	InitScreen();
	generateNext();
	addBall();
	gridsize = 800.f / N;
	post = {100, 100};
	targ = {-1, -1};
}

void Game::render(int mask) {
	if(WindowShouldClose()) exit(0);

	// 使用 bloom 渲染系统
	if(!(mask & 1)) BeginBloomRender();

	for(int i=0;i<N;++i)
		for(int j=0;j<N;++j)
			map[i][j].innerG.setTar(0), map[i][j].innerR.setTar(0);

	if(targ.x != -1) {
		auto [st, res] = map.getPath(targ, cursor);
		if(st) for(auto& p: res) map[p].innerG.setTar(.1);
		else map[targ].innerR.setTar(.5), map[cursor].innerR.setTar(.5);
	}

	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < N; ++j) {
			auto& cell = map[i][j]; auto p = post + fcord(gridsize*i, gridsize*j);

			cell.upd();
			drawCellBox(p, cell);
			if(cell.type) drawCellBall(p, cell);
		}
	}

	for(int i = 0; i < (int)particals.size(); ++i) {
		auto& par = particals[i];
		par.upd();
		if(par.size < .5) {
			std::swap(par, particals.back());
			particals.pop_back();
			--i; continue;
		}
		Color color = colorMix(ballColor[par.color], WHITE, .5);
		color.a = 192;
		DrawCircleV({par.pos.x, par.pos.y}, par.size, color);
	}

	for(int i = 0; i < ballCnt; ++i) {
		auto p = fcord(95+(i+6)*gridsize, 5);
		Cell temp; temp.type = next[i];
		drawCellBall(p, temp);
	}

	score.upd();
	char text[0xFF];
	sprintf(text, "SCORE: %d/1024", (int)ceil(score.getVal()));
	DrawText(text, 100, 40, 35, GRAY);

	if(ended) {
		DrawRectangleRec({100, 100, 800, 800}, {0, 0, 0, 192});
		auto w = MeasureText(present.data(), 200);
		DrawText(present.data(), 500 - w/2, 500 - 50/2, 50, WHITE);
	}

	Color rainbowc = ColorFromHSV(GetTime() * 10, .5, .5);
	DrawRectangleRounded({100 + 10, 900 + 10, 800 - 20, 100 - 20}, .4, 16, DARKGRAY);
	float ratio = score.getVal() / 1024, width = (800 - 30) * ratio;
	float roundness;
	if(width < 100 - 20) roundness = (.4 * (100 - 20) - 5) / width;
	else roundness = .4;
	DrawRectangleRounded({100 + 15, 900 + 15, (800 - 30) * ratio, 100 - 30}, .4, 16, ballColor[0]);


	DrawFPS(920, 980);
	if(!(mask & 2)) EndBloomRender();
}

void Game::updateEffect() {
	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < N; ++j) {
			icord np(i, j);
			auto& cell = map[np];
			if(np == cursor) cell.highlight.setTar(1), cell.highlight.setVal(1);
			else cell.highlight.setTar(0);
			if(cell.type) {
				if(np == cursor || np == targ) {
					cell.shift.setTar(10);
				} else cell.shift.setTar(0);
				if(np == targ) cell.size.setTar((sin(GetTime()*3)+1)*3+4);
				else cell.size.setTar(0);
			}
		}
	}
}

void Game::generateNext() {
	next.clear();
	for(int t = ballCnt; t--; ) {
		next.push_back(rnd(1, typeCnt));
	}
}

void Game::addBall() {
	std::vector<icord> emp;
	for(int i=0;i<N;++i) for(int j=0;j<N;++j)
		if(map[i][j].type == 0) emp.emplace_back(i, j);
	for(auto& typ: next) {
		if(emp.empty() && !checkClear()) { ended = 1, present = "GameOver!"; return ; }
		int p = rnd(0, emp.size() - 1);
		auto[x, y] = emp[p];
		map[x][y].rotation.setVal(0), map[x][y].rotation.setTar(0);
		map[x][y].type = typ;
		map[x][y].size.setVal(-25);
		emp.erase(emp.begin() + p);
	}
	generateNext();
}

void Game::attemptMove(icord s, icord t) {
	auto [st, res] = map.getPath(s, t);
	if(!st) return ;
	targ = {-1, -1};
	
	Cell movedCell; std::swap(movedCell, map[s]);
	float expectTime = std::max(.2f, (res.size() - 1) / 20.f), usedTime = 0;
	
	while(usedTime + 1 / 120.f < expectTime) {
		usedTime += 1 / 120.f;

		interact(0);
		updateEffect();
		render(2);
			static auto easeInOutCubic = [](float x) -> float {
				return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
			};
			float prog = (res.size() - 1) * easeInOutCubic(usedTime / expectTime);
			int id = prog; float lef = prog - id;
			auto p = res[id] + (res[id+1] - res[id]) * lef;
			drawCellBall(p * gridsize + post, movedCell);
		EndBloomRender();
	}
	std::swap(movedCell, map[res.back()]);
	if(int cnt = checkClear(); !cnt) {
		addBall();
		checkClear();
	} else {
		score.setTar(score.getTar() + pow(2, cnt));
		if(score.getTar() >= 1024) {
			ended = 1;
			present = "YOU WON!!!!!";
		}
	}
}

int Game::checkClear() {
	auto [cnt, res] = map.check();
	
	auto generateParticle = [&](fcord pos, Cell& cell) {
		static const float pi = acos(-1);
		float t = rnd(0, 2e8) * pi / 1e8, k = rnd(2e8, 1e9) / 1e8;
		pos = pos + fcord(45, 45 - cell.shift.getVal());
		particals.push_back(Partical{
			pos, fcord(cos(t), sin(t)) * k, rnd(200, 800) / 100.f, cell.type, 1
		});
	};
	
	for(auto& [x, y]: res) {
		for(int t = 100; t--; ) {
			generateParticle(fcord(x *gridsize, y *gridsize) + post, map[x][y]);
		}
		map[x][y] = Cell();
	}
	return res.size();
}
