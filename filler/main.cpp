#include "DxLib.h"

int fill(int x, int y, unsigned int color);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(300, 300, 32);      // 画面サイズを300x300に設定

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ChangeWindowMode(TRUE);          // ウィンドウモードに設定（フルスクリーンにしない）

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		DrawBox(100, 100, 199, 199, GetColor(255, 255, 255), FALSE); // 300x300の四角の枠を描画
		ScreenFlip();
	}

	DxLib_End();

	return 0;
}

int fill(int x, int y, unsigned int color) { return 0; }
