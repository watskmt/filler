#include "DxLib.h"
#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400


int cells[YSIZE][XSIZE] = { 0 }; // セルを初期化
int count = 0;
unsigned int wallColor;
unsigned int pathColor;

int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
int fill1(int x, int y, unsigned int color, unsigned int targetColor);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

// 迷路生成関数の宣言
void GenerateMaze();
void InitializeMaze();
void CarvePath(int x, int y);
bool IsVisited(int x, int y);
void CreateRooms();
struct Node { int x; int y; };
void CarveRoom(int x, int y, int width, int height);
void FindShortestPath();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);      // 画面サイズをWINDOW_WIDTHxWINDOW_HEIGHTに設定

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
	CreateRooms(); // 部屋を作成
	DrawCells();
	FindShortestPath();
	DrawCells();

	int x0 = 1; // 塗りつぶしの起点のx座標
	int y0 = 1; // 塗りつぶしの起点のy座標
	unsigned int targetColor = GetPixel1(x0, y0);

	while ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0);

	GetMousePoint(&x0, &y0); // マウスの位置を取得
	x0 = x0 * XSIZE / WINDOW_WIDTH; // マウスのx座標をセル座標に変換
	y0 = y0 * YSIZE / WINDOW_HEIGHT; // マウスのy座標をセル座標に変換

	fill1(x0, y0, GetColor(255, 0, 0), targetColor); // 起点を赤色で塗りつぶす

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);

	DxLib_End();

	return 0;
}

// x､yが範囲内かを返す
int outofBounds1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1; // 範囲外の場合は1を返す
	return 0; // 範囲内の場合は0を返す
}



int fill1(int x, int y, unsigned int color, unsigned int targetColor) {
	// ここに塗りつぶしロジックを作成
	unsigned int wall = GetColor(0, 0, 255);

	// 範囲外
	if (outofBounds1(x, y))
		return 0;

	// 壁
	if (GetPixel1(x, y) == wall)
		return 0;

	// 既に塗った
	if (GetPixel1(x, y) == color)
		return 0;

	DrawPixel1(x, y, color);
	DrawCells();

	fill1(x + 1, y, color, targetColor);
	fill1(x - 1, y, color, targetColor);
	fill1(x, y + 1, color,targetColor);
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
		}
	}
	if (cells[YSIZE - 2][XSIZE - 2] != pathColor)
		DrawFormatString(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, GetColor(255, 255, 255), "finished %d steps", count);
}
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color) {

	// クリップ
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= XSIZE) x2 = XSIZE - 1;
	if (y2 >= YSIZE) y2 = YSIZE - 1;

	// 無効な矩形は何もしない
	if (x1 > x2 || y1 > y2) return;

	// 直接 cells に色をセット（DrawPixel1 を経由せずに高速に設定）
	for (int x = x1; x <= x2; x++)
		cells[y1][x] = color;
	for (int x = x1; x <= x2; x++)
		cells[y2][x] = color;
	for (int y = y1; y <= y2; y++)
		cells[y][x1] = color;
	for (int y = y1; y <= y2; y++)
		cells[y][x2] = color;

	// 変更を画面に反映する場合は DrawCells() を呼ぶ
	DrawCells();
}


// 迷路を初期化（全て壁で埋める）
void InitializeMaze() {
	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {
			cells[y][x] = wallColor; // wallColor = 壁
		}
	}
}

// セルが訪問済みかチェック
bool IsVisited(int x, int y) {
	if (outofBounds1(x, y)) return true; // 範囲外は訪問済みとみなす
	return cells[y][x] == pathColor; // pathColor = 通路（訪問済み）
}

// 迷路を掘る（再帰的バックトラッキング法）
void CarvePath(int x, int y) {
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

// 迷路内に複数の部屋を作成（2x2, 2x3, 3x2, 3x3のサイズのみ）
void CreateRooms() {
	int numRooms = 5; // 部屋の数を増やす
	int roomSizes[] = { 2, 3, 4 }; // 使用可能なサイズ

	for (int i = 0; i < numRooms; i++) {
		// ランダムなサイズを選択
		int width = roomSizes[GetRand((int)(sizeof(roomSizes) / sizeof(roomSizes[0])) - 1)];
		int height = roomSizes[GetRand((int)(sizeof(roomSizes) / sizeof(roomSizes[0])) - 1)];

		// ランダムな配置位置（奇数座標に配置して壁を保つ）
		int x = GetRand((XSIZE - width) / 2) * 2 + 1;
		int y = GetRand((YSIZE - height) / 2) * 2 + 1;

		// グリッド上に配置（他の部屋と重なる可能性は無視）
		if (x + width <= XSIZE && y + height <= YSIZE) {
			CarveRoom(x, y, width, height);
		}
	}
}

// 指定位置に部屋を掘る（x, y は左上座標、width x height のサイズ）
void CarveRoom(int x, int y, int width, int height) {
	// 部屋の範囲内に pathColor を設定（通路と同じ色）
	for (int py = y; py < y + height; py++) {
		for (int px = x; px < x + width; px++) {
			if (!outofBounds1(px, py)) {
				cells[py][px] = pathColor;
			}
		}
	}
}

// スタートからゴールまでの最短経路を探す関数（幅優先探索）
void FindShortestPath()
{
	// 探索済みかどうかを記録
	bool visited[YSIZE][XSIZE] = { false };

	// 1つ前にいたマスを記録
	Node parent[YSIZE][XSIZE];

	// 探索するマスを順番に入れるキュー
	Node queue[XSIZE * YSIZE];

	// キューの先頭と最後
	int head = 0;
	int tail = 0;

	// スタート地点をキューに追加
	queue[tail++] = { 1,1 };
	visited[1][1] = true;

	// 右・左・下・上への移動量
	int dx[4] = { 1,-1,0,0 };
	int dy[4] = { 0,0,1,-1 };

	// キューが空になるまで探索
	while (head != tail)
	{
		// キューの先頭を取り出す
		Node now = queue[head++];

		// ゴールに着いたら終了
		if (now.x == XSIZE - 2 && now.y == YSIZE - 2)
			break;

		// 上下左右を調べる
		for (int i = 0; i < 4; i++)
		{
			// 次に調べる座標
			int nx = now.x + dx[i];
			int ny = now.y + dy[i];

			// 範囲外なら進まない
			if (outofBounds1(nx, ny))
				continue;

			// 探索済みなら進まない
			if (visited[ny][nx])
				continue;

			// 壁なら進まない
			if (cells[ny][nx] == wallColor)
				continue;

			// 探索済みにする
			visited[ny][nx] = true;

			// どこから来たかを記録
			parent[ny][nx] = now;

			// 次に探索するためキューへ追加
			queue[tail++] = { nx,ny };
		}
	}

	// ゴールからスタートへ戻る
	Node p = { XSIZE - 2, YSIZE - 2 };

	while (!(p.x == 1 && p.y == 1))
	{
		// 最短経路を赤色にする
		cells[p.y][p.x] = GetColor(255, 0, 0);

		// 1つ前のマスへ戻る
		p = parent[p.y][p.x];
	}

	// スタート地点も赤色にする
	cells[1][1] = GetColor(255, 0, 0);
}