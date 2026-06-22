#include "DxLib.h"

int fill(int x, int y, unsigned int color);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(300, 300, 32);

    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

   
    ClearDrawScreen();

    DrawBox(100, 100, 199, 199,
        GetColor(255, 255, 255), FALSE);

    
    fill(150, 150, GetColor(255, 0, 0));

    ScreenFlip();

    
    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        Sleep(10);
    }

    DxLib_End();

    return 0;
}

int fill(int x, int y, unsigned int color)
{
    
    if (x < 0 || x >= 300 || y < 0 || y >= 300)
    {
        return 0;
    }

    
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