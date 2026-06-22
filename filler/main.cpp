#include "DxLib.h"

int mx, my;
int fill(int x, int y, unsigned int cr = GetColor(255, 255, 0));

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(300, 300, 32);      

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ChangeWindowMode(TRUE);          
	SetMouseDispFlag(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//ClearDrawScreen();
		ScreenFlip();
		DrawBox(100, 100, 199, 199, GetColor(255, 255, 255), FALSE);
		
		if (GetMouseInput() & MOUSE_INPUT_LEFT){
			GetMousePoint(&mx, &my);// マウスカーソルの座標を取得
			//マウスカーソルが枠線の中かどうかの判定
			if (mx >= 100 && mx <= 199 &&my >= 100 && my <= 199){
				fill(mx, my);// クリックした位置を塗りつぶし
			}
		}
		
	}
	 
	
	DxLib_End();

	return 0;
}
int fill(int x, int y, unsigned int cr){
	DrawPixel(x, y, cr);
	return 0;
}
