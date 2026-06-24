#include "DxLib.h"
#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400


int cells[YSIZE][XSIZE] = { 0 }; // セルを初期化

int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
int fill1(int x, int y, unsigned int color);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);      // 画面サイズをWINDOW_WIDTHxWINDOW_HEIGHTに設定

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ChangeWindowMode(TRUE);          // ウィンドウモードに設定（フルスクリーンにしない）

	ClearDrawScreen();

	DrawBox1(5, 5, 20, 20, GetColor(0, 255, 0)); // (50, 50)から(200, 200)の範囲に緑色の四角を描画
	fill1(19, 19, GetColor(255, 0, 0)); // (150, 150)を起点に赤色で塗りつぶす 
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);

	DxLib_End();

	return 0;
} 

// x､yが範囲内かを返す
int outofBounds1(int x, int y) {
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1; // 範囲外の場合は1を返す
	return 0; // 範囲内の場合は0を返す
}



int fill1(int x, int y, unsigned int color) {
	// ここに塗りつぶしロジックを作成
	if (outofBounds1(x, y))
		return 0;

	if (GetPixel1(x, y) != 0)
		return 0;

	// スマイリーの目と口
	if (
		// 左目
		(x >= 9 && x <= 10 && y >= 9 && y <= 10)

		||

		// 右目
		(x >= 15 && x <= 16 && y >= 9 && y <= 10)

		||

		// 口
		(y == 16 && x >= 10 && x <= 15)

		||

		(y == 15 && (x == 9 || x == 16))
		)
	{
		DrawPixel1(x, y, GetColor(0, 0, 0));
	}
	else
	{
		DrawPixel1(x, y, GetColor(255, 255, 255));
	}
	DrawCells();

	fill1(x + 1, y, color);
	fill1(x - 1, y, color);
	fill1(x, y + 1, color);
	fill1(x, y - 1, color);

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
	WaitTimer(50);
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