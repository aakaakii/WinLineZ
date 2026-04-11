#include "lib/map.h"
#include <queue>
#include <algorithm>

float gridsize;

static icord fw[]{
	{0, 0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}
};
std::pair<int, std::vector<icord>> Map::getPath(icord s, icord t) {
	std::queue<icord> q;
	static int pre[N][N];
	for(int i = 0; i < N; ++i) for(int j = 0; j < N; ++j) pre[i][j] = 0;
	q.push(s);
	while(!q.empty()) {
		auto p = q.front(); q.pop();
		if(p == t) {
			std::vector<icord> res;
			while(p != s) {
				res.emplace_back(p);
				auto[fx, fy] = fw[pre[p.x][p.y]];
				p.x -= fx, p.y -= fy;
			}
			res.emplace_back(s.x, s.y);
			std::reverse(res.begin(), res.end());
			return {1, res};
		}
//		std::vector<std::tuple<float, int, int>> res;
		for(int k: {1, 2, 3, 4}) {
			auto& [fx, fy] = fw[k];
			int nx = p.x + fx, ny = p.y + fy;
			if(nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
			if(cel[nx][ny].type != 0 || pre[nx][ny]) continue;
//			res.emplace_back(std::hypot(nx - t.x, ny - t.y), nx, ny);
			q.emplace(nx, ny);
			pre[nx][ny] = k;
		}
//		std::sort(res.begin(), res.end());
//		for(auto& [d, x, y]: res) q.emplace(x, y);
	}
	return {0, {}};
}

std::pair<int, std::vector<icord>> Map::check() {
	int cnt = 0;
	std::vector<icord> res;
	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < N; ++j) {
			if(!cel[i][j].type) continue;
			int v1 = 1, v2 = 1, v3 = 1, v4 = 1;
			while(i + v1 < N && cel[i+v1][j].type == cel[i][j].type) ++v1;
			if(v1 >= 5) { ++cnt; for(int k = 0; k < v1; ++k) res.emplace_back(i+k, j); }
			while(j + v2 < N && cel[i][j+v2].type == cel[i][j].type) ++v2;
			if(v2 >= 5) { ++cnt; for(int k = 0; k < v2; ++k) res.emplace_back(i, j+k); }
			while(i + v3 < N && j + v3 < N && cel[i+v3][j+v3].type == cel[i][j].type) ++v3;
			if(v3 >= 5) { ++cnt; for(int k = 0; k < v3; ++k) res.emplace_back(i+k, j+k); }
			while(i + v4 < N && j - v4>= 0 && cel[i+v4][j-v4].type == cel[i][j].type) ++v4;
			if(v4 >= 5) { ++cnt; for(int k = 0; k < v4; ++k) res.emplace_back(i+k, j-k); }
		}
	}
	std::sort(res.begin(), res.end());
	res.erase(std::unique(res.begin(), res.end()), res.end());
	return {cnt, res};
}
