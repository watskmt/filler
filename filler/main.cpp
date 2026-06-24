#include "DxLib.h"
#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int cells[YSIZE][XSIZE] = { 0 };

int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
int fill1(int x, int y, unsigned int color);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

int mx, my;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) return -1;

	SetMouseDispFlag(TRUE);
	ClearDrawScreen();

	DrawBox1(5, 5, 20, 20, GetColor(0, 255, 0));

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			GetMousePoint(&mx, &my);

			int cx = mx / (WINDOW_WIDTH / XSIZE);
			int cy = my / (WINDOW_HEIGHT / YSIZE);

			fill1(cx, cy, GetColor(255, 0, 0));

			DrawCells();
		}
	}

	DxLib_End();
	return 0;
}

int outofBounds1(int x, int y)
{
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1;
	return 0;
}

int fill1(int x, int y, unsigned int color)
{
	if (outofBounds1(x, y)) return 0;
	if (GetPixel1(x, y) == GetColor(0, 255, 0)) return 0;
	if (GetPixel1(x, y) == color) return 0;

	DrawPixel1(x, y, color);
	DrawCells();
	WaitTimer(10);

	fill1(x + 1, y, color);
	fill1(x - 1, y, color);
	fill1(x, y + 1, color);
	fill1(x, y - 1, color);

	return 0;
}

unsigned int GetPixel1(int x, int y)
{
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return -1;
	return cells[y][x];
}

void DrawPixel1(int x, int y, unsigned int color)
{
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return;
	cells[y][x] = color;
}

void DrawCells()
{
	for (int y = 0; y < YSIZE; y++)
	{
		for (int x = 0; x < XSIZE; x++)
		{
			DrawBox(x * (WINDOW_WIDTH / XSIZE), y * (WINDOW_HEIGHT / YSIZE),
				(x + 1) * (WINDOW_WIDTH / XSIZE) - 1, (y + 1) * (WINDOW_HEIGHT / YSIZE) - 1,
				cells[y][x], TRUE);
		}
	}
	WaitTimer(100);
	ScreenFlip();
}

void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color)
{
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