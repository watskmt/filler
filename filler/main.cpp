#include "DxLib.h"

int fill(int x, int y, unsigned int color);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(300, 300, 32);      // 画面サイズを300x300に設定
	ChangeWindowMode(TRUE);          // ウィンドウモードに設定（フルスクリーンにしない）

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		DrawBox(100, 100, 199, 199, GetColor(255, 255, 255), FALSE); // 300x300の四角の枠を描画

		fill(150, 150, GetColor(0, 255, 0));

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}
int outofBounds1(int x, int y)
{
	unsigned int Cr;

	// 画面の外
	if (x < 0 || x >= 300 || y < 0 || y >= 300)
	{
		return 1;
	}

	Cr = GetPixel(x, y);

	// 白い枠に当たったら範囲外
	if (Cr == GetColor(255, 255, 255))
	{
		return 1;
	}

	return 0;
}
int fill(int x, int y, unsigned int color) {

	unsigned int Cr;
	Cr=GetPixel(x, y);

	DrawPixel(x, y, color);
	while (!outofBounds1(x + 1, y))
	{
		DrawPixel(x, y, color);
		x++;
	}

	return 0;
}