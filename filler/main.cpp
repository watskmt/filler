#include "DxLib.h"
#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int cells[YSIZE][XSIZE] = { 0 }; // セルを初期化
int count = 0;
unsigned int pathColor;
unsigned int wallColor;

// プロトタイプ宣言
int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

// 迷路生成関数の宣言
void GenerateMaze();
void InitializeMaze();
void CarvePath(int x, int y);
bool IsVisited(int x, int y);
void CreateRooms();
void CarveRoom(int x, int y, int width, int height);

// 塗りつぶし関数の宣言
void fill1(int x, int y, unsigned int fillColor, unsigned int targetColor);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);     // 画面サイズを設定

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ChangeWindowMode(TRUE);          // ウィンドウモードに設定（フルスクリーンにしない）
	SetMouseDispFlag(TRUE); // マウスポインタを表示する

	ClearDrawScreen();

	pathColor = GetColor(0, 0, 0);
	wallColor = GetColor(0, 0, 255);

	GenerateMaze(); // 迷路を生成
	CreateRooms();  // 部屋を作成
	DrawCells();

	// マウス左クリックを待つ
	while ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0);

	int x0, y0;
	GetMousePoint(&x0, &y0); // マウスの位置を取得
	x0 = x0 * XSIZE / WINDOW_WIDTH;  // マウスのx座標をセル座標に変換
	y0 = y0 * YSIZE / WINDOW_HEIGHT; // マウスのy座標をセル座標に変換

	unsigned int targetColor = GetPixel1(x0, y0);
	fill1(x0, y0, GetColor(255, 0, 0), targetColor); // 起点を赤色で塗りつぶす

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);

	DxLib_End();

	return 0;
}

// x、yが範囲外かを返す（範囲外なら1）
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

// cells を更新したときに呼び出すと画面に反映される
void DrawCells() {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			DrawBox(
				x * (WINDOW_WIDTH / XSIZE),
				y * (WINDOW_HEIGHT / YSIZE),
				(x + 1) * (WINDOW_WIDTH / XSIZE) - 1,
				(y + 1) * (WINDOW_HEIGHT / YSIZE) - 1,
				cells[y][x], TRUE
			);
		}
	}
	WaitTimer(100);
	if (cells[YSIZE - 2][XSIZE - 2] != pathColor)
		DrawFormatString(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, GetColor(255, 255, 255), "finished %d steps", count);
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

// 迷路を初期化（全て壁で埋める）
void InitializeMaze() {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			cells[y][x] = wallColor;
		}
	}
}

// セルが訪問済みかチェック
bool IsVisited(int x, int y) {
	if (outofBounds1(x, y)) return true; // 範囲外は訪問済みとみなす
	return cells[y][x] == pathColor;     // pathColor = 通路（訪問済み）
}

// 迷路を掘る（再帰的バックトラッキング法）
void CarvePath(int x, int y) {
	// 現在の位置を通路にする
	cells[y][x] = pathColor;

	// 4方向（上、下、左、右）
	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };

	// ランダムに方向をシャッフル
	for (int i = 0; i < 4; i++) {
		int j = GetRand(3 - i) + i;
		int tmp_dx = dx[i], tmp_dy = dy[i];
		dx[i] = dx[j];
		dy[i] = dy[j];
		dx[j] = tmp_dx;
		dy[j] = tmp_dy;
	}

	// 4方向を試す
	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i] * 2; // 2マス先へ移動（壁を残すため）
		int ny = y + dy[i] * 2;

		// 移動先が壁（未訪問）なら
		if (!outofBounds1(nx, ny) && cells[ny][nx] == wallColor) {
			// 現在と移動先の間の壁を掘る
			cells[y + dy[i]][x + dx[i]] = pathColor;
			// 再帰的に移動先から掘り進める
			CarvePath(nx, ny);
		}
	}
}

// 迷路を生成するメイン関数
void GenerateMaze() {
	InitializeMaze(); // 全て壁で初期化
	CarvePath(1, 1); // (1, 1) から掘り始める

	// 開始地点と終了地点を通路にする
	cells[1][1] = pathColor;
	cells[YSIZE - 2][XSIZE - 2] = pathColor;
}

// 迷路内に複数の部屋を作成
void CreateRooms() {
	int numRooms = 10;
	int roomSizes[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int numSizes = (int)(sizeof(roomSizes) / sizeof(roomSizes[0]));

	for (int i = 0; i < numRooms; i++) {
		int width = roomSizes[GetRand(numSizes - 1)];
		int height = roomSizes[GetRand(numSizes - 1)];

		// 奇数座標に配置して壁を保つ
		int x = GetRand((XSIZE - width) / 2) * 2 + 1;
		int y = GetRand((YSIZE - height) / 2) * 2 + 1;

		if (x + width <= XSIZE && y + height <= YSIZE) {
			CarveRoom(x, y, width, height);
		}
	}
}

// 指定位置に部屋を掘る（x, y は左上座標、width x height のサイズ）
void CarveRoom(int x, int y, int width, int height) {
	for (int py = y; py < y + height; py++) {
		for (int px = x; px < x + width; px++) {
			if (!outofBounds1(px, py)) {
				cells[py][px] = pathColor;
			}
		}
	}
}

// 塗りつぶし（flood fill）関数
void fill1(int x, int y, unsigned int fillColor, unsigned int targetColor) {
	if (outofBounds1(x, y)) return;
	if (cells[y][x] != targetColor) return;
	if (cells[y][x] == fillColor) return;

	cells[y][x] = fillColor;
	count++;
	DrawCells();

	fill1(x + 1, y, fillColor, targetColor);
	fill1(x - 1, y, fillColor, targetColor);
	fill1(x, y + 1, fillColor, targetColor);
	fill1(x, y - 1, fillColor, targetColor);
}