#include "DxLib.h"
#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int cells[YSIZE][XSIZE] = { 0 }; // セルを初期化

// プロトタイプ宣言
int spiralFill(unsigned int color);
int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);     // 画面サイズを設定

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ChangeWindowMode(TRUE);

	ClearDrawScreen();

	// 1. 枠線を描画する
	DrawBox1(5, 5, 20, 20, GetColor(0, 255, 0));

	// 2. 枠線の内側を中央からくるくる塗りつぶす
	spiralFill(GetColor(255, 0, 0));

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);

	DxLib_End();

	return 0;
}

// 真ん中から外側へ渦巻き状に塗りつぶす関数
int spiralFill(unsigned int color) {
	int x = XSIZE / 2;
	int y = YSIZE / 2;

	int dx[] = { 1, 0, -1, 0 };
	int dy[] = { 0, 1, 0, -1 };
	int dir = 0;

	int steps = 1;
	int step_count = 0;

	// 最初の中心マスを塗る
	if (x > 5 && x < 20 && y > 5 && y < 20) {
		DrawPixel1(x, y, color);
		DrawCells();
	}

	while (1) {
		for (int i = 0; i < steps; i++) {
			x += dx[dir];
			y += dy[dir];

			// 座標が「緑の枠線の内側」だけ塗る
			if (x > 5 && x < 20 && y > 5 && y < 20) {
				// まだ塗られていない場所なら塗る
				if (GetPixel1(x, y) == 0) {
					DrawPixel1(x, y, color);
					DrawCells();
				}
			}
		}

		dir = (dir + 1) % 4;

		step_count++;
		if (step_count == 2) {
			steps++;
			step_count = 0;
		}

		// 渦巻きのサイズが画面サイズを超えたら終了
		if (steps > XSIZE && steps > YSIZE) {
			break;
		}
	}
	return 0;
}

// x、yが範囲内かを返す
int outofBounds1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1;
	return 0;
}

unsigned int GetPixel1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return -1;
	return cells[y][x];
}

void DrawPixel1(int x, int y, unsigned int color) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return;
	cells[y][x] = color;
}

// 枠線を描画する関数
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color) {
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= XSIZE) x2 = XSIZE - 1;
	if (y2 >= YSIZE) y2 = YSIZE - 1;

	if (x1 > x2 || y1 > y2) return;

	for (int x = x1; x <= x2; x++) cells[y1][x] = color;
	for (int x = x1; x <= x2; x++) cells[y2][x] = color;
	for (int y = y1; y <= y2; y++) cells[y][x1] = color;
	for (int y = y1; y <= y2; y++) cells[y][x2] = color;

	DrawCells();
}

// 画面に反映
void DrawCells() {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			DrawBox(x * (WINDOW_WIDTH / XSIZE), y * (WINDOW_HEIGHT / YSIZE), (x + 1) * (WINDOW_WIDTH / XSIZE) - 1, (y + 1) * (WINDOW_HEIGHT / YSIZE) - 1, cells[y][x], TRUE);
		}
	}
	WaitTimer(100);
}