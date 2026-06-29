#include "DxLib.h"
#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400



int cells[YSIZE][XSIZE] = { 0 }; // 繧ｻ繝ｫ繧貞・譛溷喧

int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
int fill1(int x, int y, unsigned int color);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();


int cells[YSIZE][XSIZE] = { 0 }; // セルを初期化

int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
int fill1(int x, int y, unsigned int color, unsigned int targetColor);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

// 迷路生成関数の宣言
void GenerateMaze(unsigned int pathColor, unsigned int wallColor);
void InitializeMaze(unsigned int wallColor);
void CarvePath(int x, int y, unsigned int pathColor, unsigned int wallColor);
bool IsVisited(int x, int y, unsigned int pathColor);
void CreateRooms(unsigned int pathColor);
void CarveRoom(int x, int y, int width, int height, unsigned int roomColor);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);      // 画面サイズをWINDOW_WIDTHxWINDOW_HEIGHTに設定

	


	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ChangeWindowMode(TRUE);          // ウィンドウモードに設定（フルスクリーンにしない）

	ClearDrawScreen();

<<<<<<< HEAD
	unsigned int pathColor = GetColor(0, 0, 0);
	unsigned int wallColor = GetColor(0, 0, 255);
=======
	DrawBox1(5, 5, 20, 20, GetColor(0, 255, 0)); // (50, 50)縺九ｉ(200, 200)縺ｮ遽・峇縺ｫ邱題牡縺ｮ蝗幄ｧ偵ｒ謠冗判
	fill1(15, 15, GetColor(255, 0, 0)); // (150, 150)繧定ｵｷ轤ｹ縺ｫ襍､濶ｲ縺ｧ蝪励ｊ縺､縺ｶ縺・
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);
>>>>>>> origin/ikuta

	GenerateMaze(pathColor, wallColor); // 迷路を生成
	CreateRooms(pathColor); // 部屋を作成
	DrawCells();

<<<<<<< HEAD
	int x0 = 1; // 塗りつぶしの起点のx座標
	int y0 = 1; // 塗りつぶしの起点のy座標
	unsigned int targetColor = GetPixel1(x0, y0);
	fill1(x0, y0, GetColor(255, 0, 0), targetColor); // 起点を赤色で塗りつぶす

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);

	DxLib_End();

	return 0;
}
=======

	return 0;
} 
>>>>>>> origin/ikuta

// x､yが範囲内かを返す
int outofBounds1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1; // 範囲外の場合は1を返す
	return 0; // 範囲内の場合は0を返す
}



int fill1(int x, int y, unsigned int color, unsigned int targetColor)
{
<<<<<<< HEAD
	if (outofBounds1(x, y))
	{
		return 0;
	}

	if (cells[y][x] != targetColor)
=======

	if (x < 0 || x >= 300 || y < 0 || y >= 300)
>>>>>>> origin/ikuta
	{
		return 0;
	}

<<<<<<< HEAD
	DrawPixel1(x, y, color);

	DrawCells();

	fill1(x + 1, y, color, targetColor);
	fill1(x - 1, y, color, targetColor);
	fill1(x, y + 1, color, targetColor);
	fill1(x, y - 1, color, targetColor);

	return 0;
}

// GetPixel の代わりに使用
unsigned int GetPixel1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return -1; // 範囲外の場合は-1を返す
	return cells[y][x]; // セルの値を返す
}

// DrawPixelの代わりに使用
void DrawPixel1(int x, int y, unsigned int color) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return; // 範囲外の場合は何もしない
	cells[y][x] = color; // セルに値を設定
}

//途中で画面に反映したいときに呼び出す
// cells を更新したときに呼び出すと画面に反映される
void DrawCells() {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			DrawBox(x * (WINDOW_WIDTH / XSIZE), y * (WINDOW_HEIGHT / YSIZE), (x + 1) * (WINDOW_WIDTH / XSIZE) - 1, (y + 1) * (WINDOW_HEIGHT / YSIZE) - 1, cells[y][x], TRUE); // セルの値を描画
=======

	unsigned int now = GetPixel(x, y);


	if (now == GetColor(255, 255, 255))
	{
		return 0;
	}


	if (now == color)
	{
		return 0;
	}


	DrawPixel(x, y, color);


	fill(x + 1, y, color);
	fill(x - 1, y, color);
	fill(x, y + 1, color);
	fill(x, y - 1, color);

	return 0;
}

// x・､y縺檎ｯ・峇蜀・°繧定ｿ斐☆
int outofBounds1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1; // 遽・峇螟悶・蝣ｴ蜷医・1繧定ｿ斐☆
	return 0; // 遽・峇蜀・・蝣ｴ蜷医・0繧定ｿ斐☆
}



int fill1(int x, int y, unsigned int color) {
	// 縺薙％縺ｫ蝪励ｊ縺､縺ｶ縺励Ο繧ｸ繝・け繧剃ｽ懈・
	return 0;
}

// GetPixel 縺ｮ莉｣繧上ｊ縺ｫ菴ｿ逕ｨ
unsigned int GetPixel1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return -1; // 遽・峇螟悶・蝣ｴ蜷医・-1繧定ｿ斐☆
	return cells[y][x]; // 繧ｻ繝ｫ縺ｮ蛟､繧定ｿ斐☆
}

// DrawPixel縺ｮ莉｣繧上ｊ縺ｫ菴ｿ逕ｨ
void DrawPixel1(int x, int y, unsigned int color) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return; // 遽・峇螟悶・蝣ｴ蜷医・菴輔ｂ縺励↑縺・
	cells[y][x] = color; // 繧ｻ繝ｫ縺ｫ蛟､繧定ｨｭ螳・
}

//騾比ｸｭ縺ｧ逕ｻ髱｢縺ｫ蜿肴丐縺励◆縺・→縺阪↓蜻ｼ縺ｳ蜃ｺ縺・
// cells 繧呈峩譁ｰ縺励◆縺ｨ縺阪↓蜻ｼ縺ｳ蜃ｺ縺吶→逕ｻ髱｢縺ｫ蜿肴丐縺輔ｌ繧・
void DrawCells() {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			DrawBox(x * (WINDOW_WIDTH / XSIZE), y * (WINDOW_HEIGHT / YSIZE), (x + 1) * (WINDOW_WIDTH / XSIZE) - 1, (y + 1) * (WINDOW_HEIGHT / YSIZE) - 1, cells[y][x], TRUE); // 繧ｻ繝ｫ縺ｮ蛟､繧呈緒逕ｻ
>>>>>>> origin/ikuta
		}
	}
	WaitTimer(100);
}
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color) {

<<<<<<< HEAD
	// クリップ
=======
	// 繧ｯ繝ｪ繝・・
>>>>>>> origin/ikuta
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= XSIZE) x2 = XSIZE - 1;
	if (y2 >= YSIZE) y2 = YSIZE - 1;

<<<<<<< HEAD
	// 無効な矩形は何もしない
	if (x1 > x2 || y1 > y2) return;

	// 直接 cells に色をセット（DrawPixel1 を経由せずに高速に設定）
=======
	// 辟｡蜉ｹ縺ｪ遏ｩ蠖｢縺ｯ菴輔ｂ縺励↑縺・
	if (x1 > x2 || y1 > y2) return;

	// 逶ｴ謗･ cells 縺ｫ濶ｲ繧偵そ繝・ヨ・・rawPixel1 繧堤ｵ檎罰縺帙★縺ｫ鬮倬溘↓險ｭ螳夲ｼ・
>>>>>>> origin/ikuta
	for (int x = x1; x <= x2; x++)
		cells[y1][x] = color;
	for (int x = x1; x <= x2; x++)
		cells[y2][x] = color;
	for (int y = y1; y <= y2; y++)
		cells[y][x1] = color;
	for (int y = y1; y <= y2; y++)
		cells[y][x2] = color;

<<<<<<< HEAD
	// 変更を画面に反映する場合は DrawCells() を呼ぶ
	DrawCells();
}


// 迷路を初期化（全て壁で埋める）
void InitializeMaze(unsigned int wallColor) {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			cells[y][x] = wallColor; // wallColor = 壁
		}
	}
}

// セルが訪問済みかチェック
bool IsVisited(int x, int y, unsigned int pathColor) {
	if (outofBounds1(x, y)) return true; // 範囲外は訪問済みとみなす
	return cells[y][x] == pathColor; // pathColor = 通路（訪問済み）
}

// 迷路を掘る（再帰的バックトラッキング法）
void CarvePath(int x, int y, unsigned int pathColor, unsigned int wallColor) {
	// 現在の位置を通路にする
	cells[y][x] = pathColor; // pathColor = 通路

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
			CarvePath(nx, ny, pathColor, wallColor);
		}
	}
}

// 迷路を生成するメイン関数
void GenerateMaze(unsigned int pathColor, unsigned int wallColor) {
	InitializeMaze(wallColor); // 全て壁で初期化
	CarvePath(1, 1, pathColor, wallColor); // (1, 1) から掘り始める

	// 開始地点と終了地点を通路にする
	cells[1][1] = pathColor;
	cells[YSIZE - 2][XSIZE - 2] = pathColor;
}

// 迷路内に複数の部屋を作成（2x2, 2x3, 3x2, 3x3のサイズのみ）
void CreateRooms(unsigned int pathColor) {
	int numRooms = 6; // 部屋の数を増やす
	int roomSizes[] = { 2, 3, 4 }; // 使用可能なサイズ

	for (int i = 0; i < numRooms; i++) {
		// ランダムなサイズを選択
		int width = roomSizes[GetRand(1)];
		int height = roomSizes[GetRand(1)];

		// ランダムな配置位置（奇数座標に配置して壁を保つ）
		int x = GetRand((XSIZE - width) / 2) * 2 + 1;
		int y = GetRand((YSIZE - height) / 2) * 2 + 1;

		// グリッド上に配置（他の部屋と重なる可能性は無視）
		if (x + width <= XSIZE && y + height <= YSIZE) {
			CarveRoom(x, y, width, height, pathColor);
		}
	}
}

// 指定位置に部屋を掘る（x, y は左上座標、width x height のサイズ）
void CarveRoom(int x, int y, int width, int height, unsigned int pathColor) {
	// 部屋の範囲内に pathColor を設定（通路と同じ色）
	for (int py = y; py < y + height; py++) {
		for (int px = x; px < x + width; px++) {
			if (!outofBounds1(px, py)) {
				cells[py][px] = pathColor;
			}
		}
	}
=======
	// 螟画峩繧堤判髱｢縺ｫ蜿肴丐縺吶ｋ蝣ｴ蜷医・ DrawCells() 繧貞他縺ｶ
	DrawCells();
>>>>>>> origin/ikuta
}