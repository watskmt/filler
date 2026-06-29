#include "DxLib.h"
#include <queue>

#define XSIZE 25
#define YSIZE 25
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int cells[YSIZE][XSIZE] = { 0 };

int outofBounds1(int x, int y);
unsigned int GetPixel1(int x, int y);
void DrawPixel1(int x, int y, unsigned int color);
int fill1(int x, int y, unsigned int color, unsigned int targetColor);
void DrawBox1(int x1, int y1, int x2, int y2, unsigned int color);
void DrawCells();

void GenerateMaze(unsigned int pathColor, unsigned int wallColor);
void InitializeMaze(unsigned int wallColor);
void CarvePath(int x, int y, unsigned int pathColor, unsigned int wallColor);
bool IsVisited(int x, int y, unsigned int pathColor);
void CreateRooms(unsigned int pathColor);
void CarveRoom(int x, int y, int width, int height, unsigned int roomColor);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) return -1;

	SetMouseDispFlag(TRUE);
	ClearDrawScreen();

	unsigned int pathColor = GetColor(0, 0, 0);
	unsigned int wallColor = GetColor(0, 0, 255);

	GenerateMaze(pathColor, wallColor);
	CreateRooms(pathColor);
	DrawCells();

	while ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0);

	int x0, y0;
	GetMousePoint(&x0, &y0);
	x0 = x0 * XSIZE / WINDOW_WIDTH;
	y0 = y0 * YSIZE / WINDOW_HEIGHT;

	unsigned int targetColor = GetPixel1(x0, y0);
	fill1(x0, y0, GetColor(255, 0, 0), targetColor);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);

	DxLib_End();
	return 0;
}

int outofBounds1(int x, int y)
{
	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE) return 1;
	return 0;
}

int fill1(int x, int y, unsigned int color, unsigned int targetColor)
{
	if (outofBounds1(x, y)) return 0;
	if (cells[y][x] != targetColor) return 0	;

	std::queue<std::pair<int, int>> que;
	que.push({ x, y });

	while (!que.empty())
	{
		int cx = que.front().first;
		int cy = que.front().second;
		que.pop();

		if (outofBounds1(cx, cy)) continue;
		if (cells[cy][cx] != targetColor) continue;

		cells[cy][cx] = color;
		DrawCells();
		WaitTimer(0);

		que.push({ cx + 1, cy });
		que.push({ cx - 1, cy });
		que.push({ cx, cy + 1 });
		que.push({ cx, cy - 1 });
	}

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

void InitializeMaze(unsigned int wallColor)
{
	for (int y = 0; y < YSIZE; y++)
		for (int x = 0; x < XSIZE; x++)
			cells[y][x] = wallColor;
}

bool IsVisited(int x, int y, unsigned int pathColor)
{
	if (outofBounds1(x, y)) return true;
	return cells[y][x] == pathColor;
}

void CarvePath(int x, int y, unsigned int pathColor, unsigned int wallColor)
{
	cells[y][x] = pathColor;

	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };

	for (int i = 0; i < 4; i++)
	{
		int j = GetRand(3 - i) + i;
		int tmp_dx = dx[i], tmp_dy = dy[i];
		dx[i] = dx[j]; dy[i] = dy[j];
		dx[j] = tmp_dx; dy[j] = tmp_dy;
	}

	for (int i = 0; i < 4; i++)
	{
		int nx = x + dx[i] * 2;
		int ny = y + dy[i] * 2;

		if (!outofBounds1(nx, ny) && cells[ny][nx] == wallColor)
		{
			cells[y + dy[i]][x + dx[i]] = pathColor;
			CarvePath(nx, ny, pathColor, wallColor);
		}
	}
}

void GenerateMaze(unsigned int pathColor, unsigned int wallColor)
{
	InitializeMaze(wallColor);
	CarvePath(1, 1, pathColor, wallColor);
	cells[1][1] = pathColor;
	cells[YSIZE - 2][XSIZE - 2] = pathColor;
}

void CreateRooms(unsigned int pathColor)
{
	int numRooms = 6;
	int roomSizes[] = { 2, 3, 4 };

	for (int i = 0; i < numRooms; i++)
	{
		int width = roomSizes[GetRand(1)];
		int height = roomSizes[GetRand(1)];
		int x = GetRand((XSIZE - width) / 2) * 2 + 1;
		int y = GetRand((YSIZE - height) / 2) * 2 + 1;

		if (x + width <= XSIZE && y + height <= YSIZE)
			CarveRoom(x, y, width, height, pathColor);
	}
}

void CarveRoom(int x, int y, int width, int height, unsigned int pathColor)
{
	for (int py = y; py < y + height; py++)
		for (int px = x; px < x + width; px++)
			if (!outofBounds1(px, py))
				cells[py][px] = pathColor;
}