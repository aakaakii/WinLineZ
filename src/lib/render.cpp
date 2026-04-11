#include "lib/render.h"
#include <raymath.h>
#include <rlgl.h>

// Bloom 参数默认值
float g_bloomThreshold = 0.4f;
float g_bloomIntensity = 2.f;
int g_blurIterations = 8;

// Bloom 相关资源
static RenderTexture2D sceneTarget;
static RenderTexture2D brightnessTarget;
static RenderTexture2D hBlurTarget[2];  // 双缓冲 ping-pong
static RenderTexture2D vBlurTarget[2];

static Shader brightnessExtractShader;
static Shader gaussianBlurShader;
static Shader bloomCompositeShader;

static bool bloomInitialized = false;
static int screenWidth = 0;
static int screenHeight = 0;

void InitScreen() {
	InitWindow(1000, 1000, "LineZ v1.1");
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetTargetFPS(120);
}

void InitBloom() {
	if(bloomInitialized) return;
	
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();
	
	// 创建所有 RenderTexture
	sceneTarget = LoadRenderTexture(screenWidth, screenHeight);
	brightnessTarget = LoadRenderTexture(screenWidth, screenHeight);
	hBlurTarget[0] = LoadRenderTexture(screenWidth, screenHeight);
	hBlurTarget[1] = LoadRenderTexture(screenWidth, screenHeight);
	vBlurTarget[0] = LoadRenderTexture(screenWidth, screenHeight);
	vBlurTarget[1] = LoadRenderTexture(screenWidth, screenHeight);
	
	// 加载 shader
	brightnessExtractShader = LoadShader("shaders/passthrough.vs", "shaders/brightness_extract.fs");
	gaussianBlurShader = LoadShader("shaders/passthrough.vs", "shaders/gaussian_blur.fs");
	bloomCompositeShader = LoadShader("shaders/passthrough.vs", "shaders/bloom_composite.fs");
	
	if(!brightnessExtractShader.id || !gaussianBlurShader.id || !bloomCompositeShader.id) {
		TraceLog(LOG_WARNING, "Bloom: Failed to load some shaders");
		bloomInitialized = false;
		return;
	}
	
	// 设置分辨率统一变量
	Vector2 res = {(float)screenWidth, (float)screenHeight};
	int locRes = GetShaderLocation(gaussianBlurShader, "resolution");
	SetShaderValue(gaussianBlurShader, locRes, &res, SHADER_UNIFORM_VEC2);
	
	bloomInitialized = true;
	TraceLog(LOG_INFO, "Bloom: Initialized successfully");
}

void BeginBloomRender() {
	if(!bloomInitialized) InitBloom();
	
	BeginDrawing();
	ClearBackground({0, 0, 0, 255});
	
	// 渲染到场景纹理
	BeginTextureMode(sceneTarget);
	ClearBackground({0, 0, 0, 255});
}

void EndBloomRender() {
	EndTextureMode();  // 结束 sceneTarget
	
	if(!bloomInitialized) {
		EndDrawing();
		return;
	}
	
	// Pass 1: 提取亮部
	BeginTextureMode(brightnessTarget);
	ClearBackground({0, 0, 0, 255});
	BeginShaderMode(brightnessExtractShader);
	
	int locThreshold = GetShaderLocation(brightnessExtractShader, "threshold");
	SetShaderValue(brightnessExtractShader, locThreshold, &g_bloomThreshold, SHADER_UNIFORM_FLOAT);
	
	DrawTextureRec(sceneTarget.texture, {0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height}, {0, 0}, WHITE);
	
	EndShaderMode();
	EndTextureMode();
	
	// Pass 2: 多遍高斯模糊 (ping-pong)
	Vector2 dirH = {1.0f, 0.0f};
	Vector2 dirV = {0.0f, 1.0f};
	
	int locDirection = GetShaderLocation(gaussianBlurShader, "direction");
	
	for(int i = 0; i < g_blurIterations; i++) {
		int srcIdx = i % 2;
		int dstIdx = (i + 1) % 2;
		
		// 水平模糊
		BeginTextureMode(hBlurTarget[dstIdx]);
		ClearBackground({0, 0, 0, 255});
		BeginShaderMode(gaussianBlurShader);
		SetShaderValue(gaussianBlurShader, locDirection, &dirH, SHADER_UNIFORM_VEC2);
		
		if(i == 0) {
			// 第一次从亮度图采样
			DrawTextureRec(brightnessTarget.texture, {0, 0, (float)brightnessTarget.texture.width, -(float)brightnessTarget.texture.height}, {0, 0}, WHITE);
		} else {
			// 后续从上一次垂直模糊结果采样
			DrawTextureRec(vBlurTarget[srcIdx].texture, {0, 0, (float)vBlurTarget[srcIdx].texture.width, -(float)vBlurTarget[srcIdx].texture.height}, {0, 0}, WHITE);
		}
		
		EndShaderMode();
		EndTextureMode();
		
		// 垂直模糊
		BeginTextureMode(vBlurTarget[dstIdx]);
		ClearBackground({0, 0, 0, 255});
		BeginShaderMode(gaussianBlurShader);
		SetShaderValue(gaussianBlurShader, locDirection, &dirV, SHADER_UNIFORM_VEC2);
		
		DrawTextureRec(hBlurTarget[dstIdx].texture, {0, 0, (float)hBlurTarget[dstIdx].texture.width, -(float)hBlurTarget[dstIdx].texture.height}, {0, 0}, WHITE);
		
		EndShaderMode();
		EndTextureMode();
	}
	
	// Pass 3: 最终合成
	int lastBlurIdx = (g_blurIterations - 1) % 2;
	int locBloomIntensity = GetShaderLocation(bloomCompositeShader, "bloomIntensity");
	
	BeginShaderMode(bloomCompositeShader);
	SetShaderValue(bloomCompositeShader, locBloomIntensity, &g_bloomIntensity, SHADER_UNIFORM_FLOAT);
	
	// 绑定两个纹理
	int locTex0 = GetShaderLocation(bloomCompositeShader, "texture0");
	int locTex1 = GetShaderLocation(bloomCompositeShader, "texture1");
	
	// 激活纹理单元
	rlActiveTextureSlot(0);
	rlEnableTexture(sceneTarget.texture.id);
	rlActiveTextureSlot(1);
	rlEnableTexture(vBlurTarget[lastBlurIdx].texture.id);
	
	// 设置统一变量
	int texUnit0 = 0;
	int texUnit1 = 1;
	SetShaderValue(bloomCompositeShader, locTex0, &texUnit0, SHADER_UNIFORM_INT);
	SetShaderValue(bloomCompositeShader, locTex1, &texUnit1, SHADER_UNIFORM_INT);
	
	// 绘制场景纹理
	DrawTextureRec(sceneTarget.texture, {0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height}, {0, 0}, WHITE);
	
	EndShaderMode();
	
	// 恢复默认纹理单元
	rlActiveTextureSlot(0);
	
	EndDrawing();
}

void drawCellBox(fcord p, Cell& cell) {
	float highlight = cell.highlight.getVal(), iR = cell.innerR.getVal(), iG = cell.innerG.getVal();
	float t = 0 + 4 * (1 - highlight), h = 3;
	DrawRectangleV({p.x +t, p.y +t}, {gridsize - 2*t, gridsize - 2*t}, colorMix(BLACK, WHITE, (highlight*.8+.2)));
	float ratio = 1 / (1 + iR + iG);
	DrawRectangleV({p.x +t+h, p.y +t+h}, {gridsize -2*(t+h), gridsize -2*(t+h)}, {
		(unsigned char)(ratio * iR * 255 + ratio * 32),
		(unsigned char)(ratio * iG * 255 + ratio * 32),
		(unsigned char)(ratio * 32),
	255});
}

void DrawRoundedRectRotated(fcord _center, float size, float rotation, float roundness, Color color) {
	float halfSize = size;
	float radius = halfSize * roundness;
	int segments = 16;
	Vector2 center{_center.x, _center.y};

	int totalPoints = segments * 4;
	Vector2 points[totalPoints];
	int pointIndex = 0;

	for (int corner = 0; corner < 4; corner++) {
		float cornerAngle = corner * PI / 2.0f;
		Vector2 cornerCenter = {
			((corner + 1) & 2 ? -1 : 1) * (halfSize - radius),
			(corner & 2 ? -1 : 1) * (halfSize - radius)
		};

		for (int i = 0; i < segments; i++) {
			float angle = cornerAngle + (i * (PI/2.0f) / (segments-1));
			points[pointIndex].x = cornerCenter.x + cosf(angle) * radius;
			points[pointIndex].y = cornerCenter.y + sinf(angle) * radius;
			pointIndex++;
		}
	}

	for (int i = 0; i < totalPoints; i++) {
		Vector2 rotated = Vector2Rotate(points[i], rotation);
		points[i] = Vector2Add(center, rotated);
	}

	for (int i = 0; i < totalPoints; i++) {
		int next = (i + 1) % totalPoints;
		DrawTriangle(center, points[next], points[i], color);
	}
}

void drawCellBall(fcord p, Cell& cell) {
	Color color = ballColor[cell.type];
	float size = 20 + cell.size.getVal(), shift = cell.shift.getVal(), rotation = cell.rotation.getVal();
	p = p + fcord(gridsize/2, gridsize/2);
	DrawRoundedRectRotated(p + fcord(0, 4), size * 1.2, rotation, .45, {16, 16, 16, 127});
	DrawRoundedRectRotated(p + fcord(0,-2-shift), size * 1.2, rotation, .55, colorMix(color, DARKGRAY, .4));
	DrawRoundedRectRotated(p + fcord(0,-2-shift), size, rotation, .5, colorMix(color, WHITE, .2));
}

void renderBox(fcord pos, Color color, float t) {
	DrawRectangleLinesEx({pos.x -t, pos.y -t, gridsize +2*t, gridsize +2*t}, 2*t, color);
}

void renderEdge(fcord st, fcord ed, float t, Color color) {
	DrawLineEx({st.x, st.y}, {ed.x, ed.y}, t, color);
}
